#include "States\Settings.h"
#include "States\StateManager.h"
#include "States\CleanupMacros.h"
#include "Input.h"
#include "Components.h"
#include "Registry.h"
#include "UI/UIRenderer.h"

void SettingsState::Setup()
{
	m_active = true;

	RedrawUI();
	SetupButtons();
	SetupImages();
	SetupText();
}

void SettingsState::Input()
{

}

void SettingsState::Update()
{
}

void SettingsState::ComputeShaders()
{
}

void SettingsState::SetupButtons()
{
	//Back Button
	{
		auto OnClick = [this]()
			{
				SetInMainMenu(true);
				SetInSettings(false);
				Unload();
				stateManager.menu.Setup();
			};

		auto OnHover = [this]()
			{

			};
		registry.AddComponent<UIButtonComponent>(registry.CreateEntity(), UIButton("ExMenu/BackButton.png", "", L"", OnClick, OnHover, { 0.0f, -0.8f }));
	}
}

void SettingsState::SetupImages()
{

	//registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/OptionsButtonHover.png", { (sdl.WIDTH / 2.0f) - (426.0f / 8.0f), 100.0f }, { 1.0f, 1.0f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/D0.png", { -0.5f, 0.6f }, { 1.5f, 1.5f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/D0.png", { 0.5, 0.6f }, { 1.5f, 1.5f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { 0.1f, -0.2f }));
}

void SettingsState::SetupText()
{

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"This is the settings menu!", { 0.0f, 0.6f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Fullscreen:", { -0.1f, -0.2f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Volume: 100", { 0.0f, -0.4f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Graphics: Flawless", { 0.0f, -0.6f }));
}

void SettingsState::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	CREATE_ENTITY_MAP_entities;

	for (auto entity : View<UIButtonComponent>(registry))
	{
		UIButtonComponent* b = registry.GetComponent<UIButtonComponent>(entity);
		b->button.Release();
		registry.RemoveComponent<UIButtonComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<ImageComponent>(registry))
	{
		ImageComponent* i = registry.GetComponent<ImageComponent>(entity);
		i->image.Release();
		registry.RemoveComponent<ImageComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<TextComponent>(registry))
	{
		registry.RemoveComponent<TextComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	uint16_t destCount = DestroyEntities(entities);
}