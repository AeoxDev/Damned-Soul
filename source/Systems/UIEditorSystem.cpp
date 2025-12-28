#include "../header/systems/UIEditorSystem.h"

#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "UI/UIRenderer.h"
#include "SDLHandler.h"
#include "Input.h"
#include "UIComponents.h"
#include "../header/UI/HP_BarHelper.h" // For redraw of healthbars after layout load

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

static void RecomputeNormalizedFromPixel(UIBase& b)
{
	DSBOUNDS cb = b.GetBounds(); // width==right, height==bottom
	float centerPixelX = b.m_PixelCoords.x + (cb.right * 0.5f);
	float centerPixelY = b.m_PixelCoords.y + (cb.bottom * 0.5f);

	DSFLOAT2 newNorm;
	newNorm.x = (centerPixelX / (0.5f * (float)sdl.BASE_WIDTH)) - 1.0f;
	newNorm.y = 1.0f - (centerPixelY / (0.5f * (float)sdl.BASE_HEIGHT));

	b.m_Position = newNorm;
	b.UpdateTransform();
}

// Recompute for an entire UIComponent (base image, child images, base text, child texts)
static void RecomputeComponentFromPixels(UIComponent* ui)
{
	if (!ui) return;

	RecomputeNormalizedFromPixel(ui->m_BaseImage.baseUI);

	for (uint32_t i = 0; i < ui->m_Images.size(); ++i)
		RecomputeNormalizedFromPixel(ui->m_Images[i].baseUI);

	RecomputeNormalizedFromPixel(ui->m_BaseText.baseUI);

	for (uint32_t i = 0; i < ui->m_Texts.size(); ++i)
		RecomputeNormalizedFromPixel(ui->m_Texts[i].baseUI);
}

// Recompute for all editable UIComponents (call before SaveLayout and after LoadLayout)
static void RecomputeAllUIFromPixels()
{
	for (auto entity : View<UIComponent>(registry))
	{
		UIComponent* ui = registry.GetComponent<UIComponent>(entity);
		if (ui == nullptr)
			continue;
		// Only recompute for editable elements (avoid touching runtime-only components unless desired)
		if (!ui->IsEditable())
			continue;

		RecomputeComponentFromPixels(ui);
	}
}
/*
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
		// Recompute HP bar anchors so runtime layout respects the edited positions
		RecomputeAllHPBarAnchors();
		// clear selection/interaction state
		s_selected = EntityID();
		s_dragging = false;
		s_resizing = false;
		RedrawUI();
	}
}
*/

void UIEditorSystem::SetEditMode(bool enabled)
{
	if (enabled == s_uiEditMode)
		return;

	s_uiEditMode = enabled;

	if (s_uiEditMode)
	{
		// Entering edit mode: load saved layout if present
		LoadLayout();
		// Ensure child/usability normalized positions are consistent with pixels
		RecomputeAllUIFromPixels();
		// Ensure UI redraw
		RedrawUI();
	}
	else
	{
		// Exiting edit mode: recompute normalized positions from any pixel-only edits,
		// then save the layout. This ensures SaveLayout persists what the user sees.
		RecomputeAllUIFromPixels();
		SaveLayout();
		// Recompute HP bar anchors so runtime layout respects the edited positions
		RecomputeAllHPBarAnchors();
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

EntityID UIEditorSystem::GetSelectedEntity()
{
	// s_selected is file-local static in this translation unit; return it here.
	return s_selected;
}

void UIEditorSystem::SaveLayout(const char* filepath)
{
	std::ofstream ofs(filepath, std::ios::out | std::ios::trunc);
	if (!ofs.is_open())
		return;

	// Format:
	// If UIComponent has a persistent layout id: "<layoutId> px py sx sy"
	// Otherwise fallback to "filename#occurrence px py sx sy"
	std::unordered_map<std::string, int> counters;

	for (auto entity : View<UIComponent>(registry))
	{
		UIComponent* ui = registry.GetComponent<UIComponent>(entity);
		if (ui == nullptr)
			continue;

		if (!ui->IsEditable())
			continue;

		// Prefer explicit layout id when present
		if (ui->HasLayoutId())
		{
			std::string id = std::string(ui->GetLayoutId().begin(), ui->GetLayoutId().end());
			if (id.empty())
				continue;

			DSFLOAT2 pos = ui->m_BaseImage.baseUI.GetPosition();
			DSFLOAT2 scale = ui->m_BaseImage.baseUI.GetScale();
			ofs << id << " " << pos.x << " " << pos.y << " " << scale.x << " " << scale.y << "\n";
			continue;
		}

		// Fallback: use filename occurrence key (legacy)
		std::string name = std::string(ui->m_BaseImage.m_fileName.begin(), ui->m_BaseImage.m_fileName.end());
		if (name.empty())
			continue;

		int occurrence = ++counters[name]; // 1-based
		DSFLOAT2 pos = ui->m_BaseImage.baseUI.GetPosition();
		DSFLOAT2 scale = ui->m_BaseImage.baseUI.GetScale();

		ofs << name << "#" << occurrence << " " << pos.x << " " << pos.y << " " << scale.x << " " << scale.y << "\n";
	}
	ofs.close();
}

void UIEditorSystem::LoadLayout(const char* filepath)
{
	std::ifstream ifs(filepath);
	if (!ifs.is_open())
		return;

	// Read into memory map keyed by token (either layoutId or filename#occurrence)
	struct Entry { float px, py, sx, sy; };
	std::unordered_map<std::string, Entry> map;
	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;
		std::istringstream ss(line);
		std::string token;
		Entry e;
		if (!(ss >> token >> e.px >> e.py >> e.sx >> e.sy))
			continue;

		// store token verbatim; token can be layoutId (no '#') or legacy "name#N"
		map.emplace(token, e);
	}
	ifs.close();

	// First pass: apply entries with explicit layoutId
	for (auto entity : View<UIComponent>(registry))
	{
		UIComponent* ui = registry.GetComponent<UIComponent>(entity);
		if (ui == nullptr)
			continue;

		if (!ui->IsEditable())
			continue;

		if (!ui->HasLayoutId())
			continue;

		std::string id = std::string(ui->GetLayoutId().begin(), ui->GetLayoutId().end());
		if (id.empty())
			continue;

		auto it = map.find(id);
		if (it == map.end())
			continue;

		// Apply normalized position and scale to base image and cascade to children
		DSFLOAT2 newPos = { it->second.px, it->second.py };
		DSFLOAT2 oldPos = ui->m_BaseImage.baseUI.GetPosition();
		DSFLOAT2 delta = { newPos.x - oldPos.x, newPos.y - oldPos.y };

		ui->m_BaseImage.baseUI.SetPosition(newPos);
		ui->m_BaseImage.baseUI.SetScale({ it->second.sx, it->second.sy });

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
	}

	// Second pass: apply legacy filename#occurrence entries to components without a layoutId.
	// We need to count occurrences in same order as creation to match old behavior.
	std::unordered_map<std::string, int> applyCounters;
	for (auto entity : View<UIComponent>(registry))
	{
		UIComponent* ui = registry.GetComponent<UIComponent>(entity);
		if (ui == nullptr)
			continue;

		if (!ui->IsEditable())
			continue;

		// Skip ones that already had an explicit layout applied
		if (ui->HasLayoutId())
			continue;

		std::string name = std::string(ui->m_BaseImage.m_fileName.begin(), ui->m_BaseImage.m_fileName.end());
		if (name.empty())
			continue;

		int occ = ++applyCounters[name]; // 1-based occurrence for this run
		std::string key = name + "#" + std::to_string(occ);

		auto it = map.find(key);
		if (it == map.end())
			continue;

		// Apply normalized position and scale to base image and cascade to children
		DSFLOAT2 newPos = { it->second.px, it->second.py };
		DSFLOAT2 oldPos = ui->m_BaseImage.baseUI.GetPosition();
		DSFLOAT2 delta = { newPos.x - oldPos.x, newPos.y - oldPos.y };

		ui->m_BaseImage.baseUI.SetPosition(newPos);
		ui->m_BaseImage.baseUI.SetScale({ it->second.sx, it->second.sy });

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
	}

	RedrawUI();
}

// Dragging doesn't stop despite releasing cursor 
/*
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

	// Dragging update (pixel-safe, keeps normalized & pixel coords in sync)
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
			// compute desired new center in pixels from mouse
			int newCenterX = uiMouseX - s_dragOffsetX;
			int newCenterY = uiMouseY - s_dragOffsetY;

			// clamp to window bounds (optional)
			newCenterX = std::clamp(newCenterX, 0, (int)sdl.BASE_WIDTH);
			newCenterY = std::clamp(newCenterY, 0, (int)sdl.BASE_HEIGHT);

			// compute previous center from base image current pixel coords
			DSFLOAT2 basePixel = ui->m_BaseImage.baseUI.GetPixelCoords();
			DSBOUNDS baseBounds = ui->m_BaseImage.baseUI.GetBounds();
			int prevCenterX = (int)(basePixel.x + baseBounds.right * 0.5f);
			int prevCenterY = (int)(basePixel.y + baseBounds.bottom * 0.5f);

			// pixel delta that we need to move everything by
			int pixelDeltaX = newCenterX - prevCenterX;
			int pixelDeltaY = newCenterY - prevCenterY;

			// If nothing to do, skip
			if (pixelDeltaX == 0 && pixelDeltaY == 0)
			{
				// still force a redraw for visual feedback
				RedrawUI();
				return true;
			}

			// Helper lambda to apply pixel delta and recompute normalized pos for a UIBase
			auto applyPixelDeltaAndRecomputeNorm = [&](UIBase& b)
				{
					// shift pixel coords
					b.m_PixelCoords.x += (float)pixelDeltaX;
					b.m_PixelCoords.y += (float)pixelDeltaY;

					// compute normalized center-based position expected by SetPosition(DSFLOAT2)
					// Note: GetBounds() returns current size in pixels (right==width, bottom==height)
					DSBOUNDS cb = b.GetBounds();
					float centerPixelX = b.m_PixelCoords.x + (cb.right * 0.5f);
					float centerPixelY = b.m_PixelCoords.y + (cb.bottom * 0.5f);

					DSFLOAT2 newNorm;
					newNorm.x = (centerPixelX / (0.5f * (float)sdl.BASE_WIDTH)) - 1.0f;
					newNorm.y = 1.0f - (centerPixelY / (0.5f * (float)sdl.BASE_HEIGHT));

					// update normalized pos (keeps both representations consistent) and update transform
					b.m_Position = newNorm;
					b.UpdateTransform();
				};

			// Move base image
			applyPixelDeltaAndRecomputeNorm(ui->m_BaseImage.baseUI);

			// Move child images
			for (uint32_t i = 0; i < ui->m_Images.size(); ++i)
			{
				applyPixelDeltaAndRecomputeNorm(ui->m_Images[i].baseUI);
			}

			// Move base text
			applyPixelDeltaAndRecomputeNorm(ui->m_BaseText.baseUI);

			// Move child texts
			for (uint32_t i = 0; i < ui->m_Texts.size(); ++i)
			{
				applyPixelDeltaAndRecomputeNorm(ui->m_Texts[i].baseUI);
			}

			// Finished moving: force redraw
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
			s_selected.index = 0;
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
*/

// Slight delay when releasing mouse but works
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
		bool found = false;
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
				found = true;
				break;
			}
		}

		// If no element under cursor, deselect
		if (!found)
		{
			s_selected = EntityID(); // clear selection when clicking empty space
			s_dragging = false;
			RedrawUI();
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

			if (ui == nullptr)
			{
				// selection is invalid, cancel resize attempt
				return true;
			}

			if (!ui->IsEditable())
				return true;

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

	// Dragging update (pixel-safe, keeps normalized & pixel coords in sync)
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

			if (!ui->IsEditable())
				return true;

			// compute desired new center in pixels from mouse
			int newCenterX = uiMouseX - s_dragOffsetX;
			int newCenterY = uiMouseY - s_dragOffsetY;

			// clamp to window bounds (optional)
			newCenterX = std::clamp(newCenterX, 0, (int)sdl.BASE_WIDTH);
			newCenterY = std::clamp(newCenterY, 0, (int)sdl.BASE_HEIGHT);

			// compute previous center from base image current pixel coords
			DSFLOAT2 basePixel = ui->m_BaseImage.baseUI.GetPixelCoords();
			DSBOUNDS baseBounds = ui->m_BaseImage.baseUI.GetBounds();
			int prevCenterX = (int)(basePixel.x + baseBounds.right * 0.5f);
			int prevCenterY = (int)(basePixel.y + baseBounds.bottom * 0.5f);

			// pixel delta that we need to move everything by
			int pixelDeltaX = newCenterX - prevCenterX;
			int pixelDeltaY = newCenterY - prevCenterY;

			// If nothing to do, skip
			if (pixelDeltaX == 0 && pixelDeltaY == 0)
			{
				// still force a redraw for visual feedback
				RedrawUI();
				return true;
			}

			// Helper lambda to apply pixel delta and recompute normalized pos for a UIBase
			auto applyPixelDeltaAndRecomputeNorm = [&](UIBase& b)
				{
					// shift pixel coords
					b.m_PixelCoords.x += (float)pixelDeltaX;
					b.m_PixelCoords.y += (float)pixelDeltaY;

					// compute normalized center-based position expected by SetPosition(DSFLOAT2)
					// Note: GetBounds() returns current size in pixels (right==width, bottom==height)
					DSBOUNDS cb = b.GetBounds();
					float centerPixelX = b.m_PixelCoords.x + (cb.right * 0.5f);
					float centerPixelY = b.m_PixelCoords.y + (cb.bottom * 0.5f);

					DSFLOAT2 newNorm;
					newNorm.x = (centerPixelX / (0.5f * (float)sdl.BASE_WIDTH)) - 1.0f;
					newNorm.y = 1.0f - (centerPixelY / (0.5f * (float)sdl.BASE_HEIGHT));

					// update normalized pos (keeps both representations consistent) and update transform
					b.m_Position = newNorm;
					b.UpdateTransform();
				};

			// Move base image
			applyPixelDeltaAndRecomputeNorm(ui->m_BaseImage.baseUI);

			// Move child images
			for (uint32_t i = 0; i < ui->m_Images.size(); ++i)
			{
				applyPixelDeltaAndRecomputeNorm(ui->m_Images[i].baseUI);
			}

			// Move base text
			applyPixelDeltaAndRecomputeNorm(ui->m_BaseText.baseUI);

			// Move child texts
			for (uint32_t i = 0; i < ui->m_Texts.size(); ++i)
			{
				applyPixelDeltaAndRecomputeNorm(ui->m_Texts[i].baseUI);
			}

			// Finished moving: force redraw
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

	// Fallback: if dragging is active but the physical left button is not down (lost release event),
	// cancel dragging to avoid "stuck drag" behavior.
	if (s_dragging)
	{
		int bx, by;
		Uint32 s = SDL_GetMouseState(&bx, &by);
		if ((s & SDL_BUTTON(SDL_BUTTON_LEFT)) == 0)
		{
			s_dragging = false;
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