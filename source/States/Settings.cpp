#include "States\Settings.h"
#include "States\StateManager.h"
#include "States\CleanupMacros.h"
#include "Input.h"
#include "Components.h"
#include "Registry.h"
#include "UI/UIRenderer.h"
#include "Camera.h"
#include "UI/UIButtonFunctions.h"

void SettingsState::Setup()
{
	m_active = true;

	RedrawUI();
	SetupButtons();
	SetupImages();
	SetupText();

	Camera::ResetCamera();
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
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("ExMenu/BackButton.png", "", L"", UIFunc::Settings_Back, { 0.0f, -0.8f });
	}
}

void SettingsState::SetupImages()
{
	auto i1 = registry.CreateEntity();
	auto ic1 = registry.AddComponent<UIImage>(i1);
	ic1->Setup("ExMenu/OptionsButtonHover.png", { (sdl.WIDTH / 2.0f) - (426.0f / 8.0f), 100.0f }, { 1.0f, 1.0f });

	auto i2 = registry.CreateEntity();
	auto ic2 = registry.AddComponent<UIImage>(i2);
	ic2->Setup("ExMenu/D0.png", { -0.5f, 0.6f }, { 1.5f, 1.5f });

	auto i3 = registry.CreateEntity();
	auto ic3 = registry.AddComponent<UIImage>(i3);
	ic3->Setup("ExMenu/D0.png", { 0.5f, 0.6f }, { 1.5f, 1.5f });

	auto i4 = registry.CreateEntity();
	auto ic4 = registry.AddComponent<UIImage>(i4);
	ic4->Setup("ExMenu/CheckBoxBase.png", { 0.1f, -0.2f });
	
	//registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/OptionsButtonHover.png", { (sdl.WIDTH / 2.0f) - (426.0f / 8.0f), 100.0f }, { 1.0f, 1.0f }));
	//registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/D0.png", { -0.5f, 0.6f }, { 1.5f, 1.5f }));
	//registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/D0.png", { 0.5, 0.6f }, { 1.5f, 1.5f }));
	//registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { 0.1f, -0.2f }));
}

void SettingsState::SetupText()
{
	auto t1 = registry.CreateEntity();
	auto tc1 = registry.AddComponent<UIText>(t1);
	tc1->Setup(L"This is the settings menu!", { 0.0f, 0.6f });

	auto t2 = registry.CreateEntity();
	auto tc2 = registry.AddComponent<UIText>(t2);
	tc2->Setup(L"Fullscreen:", { -0.1f, -0.2f });

	auto t3 = registry.CreateEntity();
	auto tc3 = registry.AddComponent<UIText>(t3);
	tc3->Setup(L"Volume: 100", { 0.0f, -0.4f });

	auto t4 = registry.CreateEntity();
	auto tc4 = registry.AddComponent<UIText>(t4);
	tc4->Setup(L"Graphics: Flawless", { 0.0f, -0.6f });

	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"This is the settings menu!", { 0.0f, 0.6f }));
	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Fullscreen:", { -0.1f, -0.2f }));
	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Volume: 100", { 0.0f, -0.4f }));
	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Graphics: Flawless", { 0.0f, -0.6f }));
}

void SettingsState::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	for (auto entity : View<UIButton>(registry))
	{
		UIButton* b = registry.GetComponent<UIButton>(entity);
		b->Release();
	}

	for (auto entity : View<UIImage>(registry))
	{
		UIImage* i = registry.GetComponent<UIImage>(entity);
		i->Release();
	}

	//Destroy entity resets component bitmasks
	for (int i = 0; i < registry.entities.size(); i++)
	{
		registry.DestroyEntity({ i, false });
	}
	
}