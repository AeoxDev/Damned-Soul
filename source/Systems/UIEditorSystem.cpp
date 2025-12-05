#include "../header/systems/UIEditorSystem.h"

#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "UI/UIRenderer.h"
#include "SDLHandler.h"
#include "Input.h"
#include "UIComponents.h"

#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <unordered_map>

static bool s_uiEditMode = false;

// Editor runtime state
static EntityID s_selected = { 0 }; // default constructed; valid test uses .index if yours provides it
static bool s_dragging = false;
static bool s_resizing = false;
static int s_dragOffsetX = 0;
static int s_dragOffsetY = 0;
static float s_startScaleX = 1.0f;
static float s_startScaleY = 1.0f;
static int s_startMouseX = 0;

static inline std::string ToStdString(const ML_String& s)
{
	return std::string(s.begin(), s.end());
}

void UIEditorSystem::SetEditMode(bool enabled)
{
	if (enabled == s_uiEditMode)
		return;

	s_uiEditMode = enabled;

	if (s_uiEditMode)
	{
		// Entering edit mode: load saved layout if present
		LoadLayout();
		// Ensure UI redraw
		RedrawUI();
	}
	else
	{
		// Exiting edit mode: save current layout
		SaveLayout();
		// clear selection/interaction state
		s_selected = EntityID();
		s_dragging = false;
		s_resizing = false;
		RedrawUI();
	}
}

bool UIEditorSystem::GetEditMode()
{
	return s_uiEditMode;
}

void UIEditorSystem::SaveLayout(const char* filepath)
{
	std::ofstream ofs(filepath, std::ios::out | std::ios::trunc);
	if (!ofs.is_open())
		return;

	// Format: filename posX posY scaleX scaleY
	for (auto entity : View<UIComponent>(registry))
	{
		UIComponent* ui = registry.GetComponent<UIComponent>(entity);
		if (ui == nullptr)
			continue;

		// Use base image filename as identifier (best-effort)
		std::string name = ToStdString(ui->m_BaseImage.m_fileName);
		if (name.empty())
			continue;

		// Get normalized position (center) and scale
		DSFLOAT2 pos = ui->m_BaseImage.baseUI.GetPosition();
		DSFLOAT2 scale = ui->m_BaseImage.baseUI.GetScale();

		ofs << name << " " << pos.x << " " << pos.y << " " << scale.x << " " << scale.y << "\n";
	}
	ofs.close();
}

void UIEditorSystem::LoadLayout(const char* filepath)
{
	std::ifstream ifs(filepath);
	if (!ifs.is_open())
		return;

	// Read into memory map for quick lookup
	struct Entry { float px, py, sx, sy; };
	std::unordered_map<std::string, Entry> map;
	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;
		std::istringstream ss(line);
		std::string name;
		Entry e;
		if (!(ss >> name >> e.px >> e.py >> e.sx >> e.sy))
			continue;
		map.emplace(name, e);
	}
	ifs.close();

	// Apply to UI components (first-match per name)
	for (auto entity : View<UIComponent>(registry))
	{
		UIComponent* ui = registry.GetComponent<UIComponent>(entity);
		if (ui == nullptr)
			continue;

		std::string name = ToStdString(ui->m_BaseImage.m_fileName);
		if (name.empty())
			continue;

		auto it = map.find(name);
		if (it == map.end())
			continue;

		// Apply normalized position and scale to base image and cascade to children
		DSFLOAT2 newPos = { it->second.px, it->second.py };
		DSFLOAT2 oldPos = ui->m_BaseImage.baseUI.GetPosition();
		DSFLOAT2 delta = { newPos.x - oldPos.x, newPos.y - oldPos.y };

		ui->m_BaseImage.baseUI.SetPosition(newPos);
		ui->m_BaseImage.baseUI.SetScale({ it->second.sx, it->second.sy });

		// Move texts and images by same delta
		for (uint32_t i = 0; i < ui->m_Images.size(); ++i)
		{
			DSFLOAT2 p = ui->m_Images[i].baseUI.GetPosition();
			ui->m_Images[i].baseUI.SetPosition(DSFLOAT2{ p.x + delta.x, p.y + delta.y });
			// Optionally preserve relative scale (here we leave per-image scale unchanged).
		}
		for (uint32_t i = 0; i < ui->m_Texts.size(); ++i)
		{
			DSFLOAT2 p = ui->m_Texts[i].baseUI.GetPosition();
			ui->m_Texts[i].baseUI.SetPosition(DSFLOAT2{ p.x + delta.x, p.y + delta.y });
		}
	}
	RedrawUI();
}

bool UIEditorSystem::Update()
{
	// Only active when edit mode enabled
	if (!s_uiEditMode)
		return true;

	// Convert mouse to UI pixel coordinates (same approach used across codebase)
	int uiMouseX = (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH));
	int uiMouseY = (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT));

	// Selection start: left mouse pressed -> pick topmost element under cursor
	if (mouseButtonPressed[MouseButton::left] == pressed && !s_dragging && !s_resizing)
	{
		// Find first UIComponent under cursor (honor visibility)
		for (auto entity : View<UIComponent>(registry))
		{
			UIComponent* ui = registry.GetComponent<UIComponent>(entity);
			if (!ui) continue;
			if (!ui->m_BaseImage.baseUI.GetVisibility()) continue;

			DSFLOAT2 pixel = ui->m_BaseImage.baseUI.GetPixelCoords(); // top-left in pixels
			DSBOUNDS b = ui->m_BaseImage.baseUI.GetBounds(); // width==right, height==bottom

			if (uiMouseX >= (int)pixel.x && uiMouseX <= (int)(pixel.x + b.right) &&
				uiMouseY >= (int)pixel.y && uiMouseY <= (int)(pixel.y + b.bottom))
			{
				s_selected = entity;
				// Begin drag
				s_dragging = true;
				// compute center
				int centerX = (int)(pixel.x + b.right * 0.5f);
				int centerY = (int)(pixel.y + b.bottom * 0.5f);
				s_dragOffsetX = uiMouseX - centerX;
				s_dragOffsetY = uiMouseY - centerY;
				break;
			}
		}
	}

	// If right mouse pressed start resizing (relative to horizontal movement)
	if (mouseButtonPressed[MouseButton::right] == pressed && !s_resizing && !s_dragging)
	{
		// try select as above if none selected
		if (s_selected.index == 0)
		{
			for (auto entity : View<UIComponent>(registry))
			{
				UIComponent* ui = registry.GetComponent<UIComponent>(entity);
				if (!ui || !ui->m_BaseImage.baseUI.GetVisibility()) continue;
				DSFLOAT2 pixel = ui->m_BaseImage.baseUI.GetPixelCoords();
				DSBOUNDS b = ui->m_BaseImage.baseUI.GetBounds();
				if (uiMouseX >= (int)pixel.x && uiMouseX <= (int)(pixel.x + b.right) &&
					uiMouseY >= (int)pixel.y && uiMouseY <= (int)(pixel.y + b.bottom))
				{
					s_selected = entity;
					break;
				}
			}
		}

		if (s_selected.index != 0)
		{
			UIComponent* ui = registry.GetComponent<UIComponent>(s_selected);
			if (ui)
			{
				s_resizing = true;
				DSFLOAT2 scale = ui->m_BaseImage.baseUI.GetScale();
				s_startScaleX = scale.x;
				s_startScaleY = scale.y;
				s_startMouseX = uiMouseX;
			}
		}
	}

	// Dragging update
	if (s_dragging && s_selected.index != 0)
	{
		UIComponent* ui = registry.GetComponent<UIComponent>(s_selected);
		if (ui == nullptr)
		{
			s_dragging = false;
			s_selected = EntityID();
		}
		else
		{
			// compute new center pixel
			int centerX = uiMouseX - s_dragOffsetX;
			int centerY = uiMouseY - s_dragOffsetY;

			// clamp to window bounds (optional)
			centerX = std::clamp(centerX, 0, (int)sdl.BASE_WIDTH);
			centerY = std::clamp(centerY, 0, (int)sdl.BASE_HEIGHT);

			// normalized coords expected by SetPosition(DSFLOAT2) are center-based
			DSFLOAT2 newNorm;
			newNorm.x = (float(centerX) / (0.5f * (float)sdl.BASE_WIDTH)) - 1.0f;
			newNorm.y = 1.0f - (float(centerY) / (0.5f * (float)sdl.BASE_HEIGHT));

			// compute delta in normalized space and shift children
			DSFLOAT2 oldNorm = ui->m_BaseImage.baseUI.GetPosition();
			DSFLOAT2 delta = { newNorm.x - oldNorm.x, newNorm.y - oldNorm.y };

			// apply to base
			ui->m_BaseImage.baseUI.SetPosition(newNorm);

			// apply same delta to other images/texts
			for (uint32_t i = 0; i < ui->m_Images.size(); ++i)
			{
				DSFLOAT2 p = ui->m_Images[i].baseUI.GetPosition();
				ui->m_Images[i].baseUI.SetPosition(DSFLOAT2{ p.x + delta.x, p.y + delta.y });
			}
			for (uint32_t i = 0; i < ui->m_Texts.size(); ++i)
			{
				DSFLOAT2 p = ui->m_Texts[i].baseUI.GetPosition();
				ui->m_Texts[i].baseUI.SetPosition(DSFLOAT2{ p.x + delta.x, p.y + delta.y });
			}

			RedrawUI();
		}
	}

	// Resizing update (horizontal drag changes scale proportionally)
	if (s_resizing && s_selected.index != 0)
	{
		UIComponent* ui = registry.GetComponent<UIComponent>(s_selected);
		if (ui == nullptr)
		{
			s_resizing = false;
			s_selected = EntityID();
		}
		else
		{
			int dx = uiMouseX - s_startMouseX;
			float factor = 1.0f + (float)dx / 1500.0f; // tuning constant
			factor = (std::max)(0.1f, factor);

			ui->m_BaseImage.baseUI.SetScale(DSFLOAT2{ s_startScaleX * factor, s_startScaleY * factor });

			// optionally scale children uniformly
			for (uint32_t i = 0; i < ui->m_Images.size(); ++i)
			{
				DSFLOAT2 sc = ui->m_Images[i].baseUI.GetScale();
				ui->m_Images[i].baseUI.SetScale(DSFLOAT2{ sc.x * factor, sc.y * factor });
			}
			for (uint32_t i = 0; i < ui->m_Texts.size(); ++i)
			{
				DSFLOAT2 sc = ui->m_Texts[i].baseUI.GetScale();
				ui->m_Texts[i].baseUI.SetScale(DSFLOAT2{ sc.x * factor, sc.y * factor });
			}
			RedrawUI();
		}
	}

	// Mouse releases end interactions
	if (mouseButtonPressed[MouseButton::left] == released)
	{
		if (s_dragging)
		{
			s_dragging = false;
			// Optionally auto-save here or leave for user
			RedrawUI();
		}
	}

	if (mouseButtonPressed[MouseButton::right] == released)
	{
		if (s_resizing)
		{
			s_resizing = false;
			// Optionally auto-save here
			RedrawUI();
		}
	}

	// Draw selection feedback: we don't change renderer here; UIRenderer draws all UI.
	// Instead we can nudge the selected element's base opacity or similar to highlight.
	// For now we toggle base text visibility (non-destructive) to ensure re-draw.
	if (s_selected.index != 0)
	{
		UIComponent* ui = registry.GetComponent<UIComponent>(s_selected);
		if (ui)
		{
			// ensure visible while editing
			ui->m_BaseImage.baseUI.SetVisibility(true);
			// We could also draw outlines using D2D directly in UIRenderer if desired.
		}
	}

	return true;
}