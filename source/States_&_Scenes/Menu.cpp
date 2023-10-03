#include "States_&_Scenes\Menu.h"
#include "UIRenderer.h"
#include "Menus.h"
#include "Hitbox.h"
#include "States_&_Scenes\StateManager.h"
#include "Input.h"
#include "Registry.h"
#include "Components.h"


void Menu::ComputeShaders()
{
	//Particles::PrepareParticleCompute();
	//Dispatch(1, 2, 0);
	//Particles::FinishParticleCompute();
}

void Menu::Update()
{

}

void Menu::Input()
{

}

void Menu::Setup()//Load
{
	SetupButtons();
	SetupImages();
	SetupText();

	DrawUi();
}

void Menu::SetupButtons()
{
	//Start Button
	{
		auto OnClick = [this]()
			{
				SetInPlay(true);
				SetInMainMenu(false);
				stateManager.levelScenes[0].Setup(0);
				Unload();
			};

		auto OnHover = [this]()
			{

			};

		ButtonComponent* c = registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton(ui, "Exmenu/StartButton.png", "", L"", OnClick, OnHover, { 0.0f, 0.0f }));
		int ase = 2;
	}

	//Options Button
	{
		auto OnClick = [this]()
			{
				SetInSettings(true);
				SetInMainMenu(false);
				stateManager.settings.Setup();
				Unload();
			};

		auto OnHover = [this]()
			{

			};

		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton(ui, "Exmenu/OptionsButton.png", "", L"", OnClick, OnHover, { 0.0f,  -0.1f}));
	}

	//Exit Button
	{
		auto OnClick = [this]()
			{
				
			};

		auto OnHover = [this]()
			{

			};

		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton(ui, "Exmenu/ExitButton.png", "", L"", OnClick, OnHover, { 0.0f, -0.2f }));
	}
}

void Menu::SetupImages()
{
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/ExTitle.png", { 0.0f, 0.9f }, { 4.0f, 4.0f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/Eye.png", { 50.0f, 50.0f }, { 1.5f, 1.5f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/Eye.png", { sdl.WIDTH - 68.0f, 50.0f }, { 1.5f, 1.5f }));
}

void Menu::SetupText()
{
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"This is the main menu!", { sdl.WIDTH / 2.0f - 50.0f, 300.0f }));
}

void Menu::Render()
{
	/*for (auto entity : View<UIRenderComponents>(registry))
	{
		UIRenderComponents* uiComp = registry.GetComponent<UIRenderComponents>(entity);
		RenderUI(uiComp);
	}*/
	RenderUI();
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
	ClearUI();
}

void Menu::DrawUi()
{
	ID2D1RenderTarget* rt = ui.GetRenderTarget();

	UpdateUI2();

	Begin2dFrame(ui);

	for (auto entity : View<ButtonComponent>(registry))
		registry.GetComponent<ButtonComponent>(entity)->button.Draw(ui, rt);

	for (auto entity : View<ImageComponent>(registry))
		registry.GetComponent<ImageComponent>(entity)->image.Draw(rt);

	for (auto entity : View<TextComponent>(registry))
		registry.GetComponent<TextComponent>(entity)->text.Draw(ui);

	End2dFrame(ui);
}