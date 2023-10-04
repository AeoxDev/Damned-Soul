#include "States\Menu.h"
#include "UIRenderer.h"
#include "Hitbox.h"
#include "States\StateManager.h"
#include "Input.h"
#include "Registry.h"
#include "Components.h"

void Menu::Setup()//Load
{
	RedrawUI();
	SetupButtons();
	SetupImages();
	SetupText();
}

void Menu::Input()
{
	//Particles::PrepareParticleCompute();
	//Dispatch(1, 2, 0);
	//Particles::FinishParticleCompute();
}

void Menu::Update()
{

}

void Menu::ComputeShaders()
{

}

void Menu::SetupButtons()
{
	//Start Button
	{
		auto OnClick = [this]()
			{
				SetInPlay(true);
				SetInMainMenu(false);
				Unload();
				stateManager.levelScenes[0].Setup(0);
			};

		auto OnHover = [this]()
			{

			};

		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("Exmenu/StartButton.png", "", L"", OnClick, OnHover, { 0.0f, -0.4f }));
	}

	//Options Button
	{
		auto OnClick = [this]()
			{
				SetInSettings(true);
				SetInMainMenu(false);
				Unload();
				stateManager.settings.Setup();
			};

		auto OnHover = [this]()
			{

			};

		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("Exmenu/OptionsButton.png", "", L"", OnClick, OnHover, { 0.0f,  -0.6f}));
	}

	//Exit Button
	{
		auto OnClick = [this]()
			{
				
			};

		auto OnHover = [this]()
			{

			};

		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("Exmenu/ExitButton.png", "", L"", OnClick, OnHover, { 0.0f, -0.8f }));
	}
}

void Menu::SetupImages()
{
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/ExTitle.png", { 0.0f, 0.6f }, { 2.0f, 2.0f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/Eye.png", { -0.8f, 0.6f }, { 1.5f, 1.5f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/Eye.png", { 0.8f, 0.6f }, { 1.5f, 1.5f }));
}

void Menu::SetupText()
{
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"This is the main menu!", { 0.0f, 0.0f }));
}

void Menu::Unload()
{
	for (auto entity : View<ButtonComponent>(registry))
	{
		registry.RemoveComponent<ButtonComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<ImageComponent>(registry))
	{
		registry.RemoveComponent<ImageComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<TextComponent>(registry))
	{
		registry.RemoveComponent<TextComponent>(entity);
		registry.DestroyEntity(entity);
	}

	ClearUI();
}