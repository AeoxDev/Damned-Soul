#include "States_&_Scenes\Menu.h"
#include "UIRenderer.h"
#include "MainMenu.h"
#include "Hitbox.h"
#include "States_&_Scenes\StateManager.h"
#include "Input.h"
//
//void Menu::Update()
//{
//	//std::cout << "menu" << std::endl;
//}
//
//void Menu::ReadKeyInputs(int keyState[], Settings& settings)
//{
//	switch (currentSubState)
//	{
//	case MenuState::Main:
//		std::cout << "Main\n";
//
//		break;
//	case MenuState::Settings:
//		std::cout << "Settings\n";
//
//		if (keyState[SDL_SCANCODE_ESCAPE])
//			currentSubState = MenuState::Main;
//
//		settings.ReadKeyInputs(keyState);
//		break;
//	case MenuState::Credits:
//		std::cout << "Credits\n";
//
//		if (keyState[SDL_SCANCODE_ESCAPE])
//			currentSubState = MenuState::Main;
//
//		break;
//	}
//}
//
//void Menu::ReadKeyOutputs(int keyState[], Settings& settings)
//{
//	switch (currentSubState)
//	{
//	case MenuState::Main:
//		std::cout << "Main\n";
//
//		break;
//	case MenuState::Settings:
//		std::cout << "Settings\n";
//
//		break;
//	case MenuState::Credits:
//		std::cout << "Credits\n";
//
//		break;
//	}
//}
//
//void Menu::ReadMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos)
//{
//	switch (currentSubState)
//	{
//	case MenuState::Main:
//		std::cout << "Main\n";
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Play").Intersects(mousePos))
//			buttonManager.DoButtonAction("Play");
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("MenuSettings").Intersects(mousePos))
//			buttonManager.DoButtonAction("MenuSettings");
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Credits").Intersects(mousePos))
//			buttonManager.DoButtonAction("Credits");
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Exit").Intersects(mousePos))
//			buttonManager.DoButtonAction("Exit");
//
//		break;
//	case MenuState::Settings:
//		std::cout << "Settings\n";
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Main").Intersects(mousePos))
//			buttonManager.DoButtonAction("Main");
//
//		settings.ReadMouseInputs(mouseEvent, buttonManager, mousePos);
//		break;
//	case MenuState::Credits:
//		std::cout << "Credits\n";
//
//		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Main").Intersects(mousePos))
//			buttonManager.DoButtonAction("Main");
//
//		break;
//	}
//}
//
//void Menu::ReadMouseOutputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos)
//{
//	switch (currentSubState)
//	{
//	case MenuState::Main:
//		std::cout << "Main\n";
//
//		break;
//	case MenuState::Settings:
//		std::cout << "Settings\n";
//
//		break;
//	case MenuState::Credits:
//		std::cout << "Credits\n";
//
//		break;
//	}
//}

void Menu::ComputeShaders()
{

}

void Menu::Update()
{

}

void Menu::Input()
{
	//Input controller component.
	if (keyState[SDL_SCANCODE_1] == released)
	{
		SetInPlay(true);
		SetInMainMenu(false);
		stateManager.levelScenes[0].Setup(0);
		Unload();
	}
}

void Menu::Setup()//Load
{
	//Add entities and components to the registry for the main menu here
	
	//Entities, pageComponent (active, priority)
	EntityID mainMenuPage = registry.CreateEntity();

	this->registry.AddComponent<UICanvas>(mainMenuPage);
	UICanvas* mainMenu = registry.GetComponent<UICanvas>(mainMenuPage);
	SetupMainMenuCanvas(*mainMenu);

	// Create UI and example menu
	//*ui = UI();
	
	//exMenu->Setup(*ui);
	////ui->SetCurrentCanvas(exMenu->m_CurrentPage);
	//DrawGUI(*mainMenu);
	Begin2dFrame(ui);
	mainMenu->Render(ui);
	End2dFrame(ui);
}

void Menu::Render()
{
	for (auto entity : View<UIRenderComponents>(registry))
	{
	/*	UIRenderComponents* uiComp = registry.GetComponent<UIRenderComponents>(entity);
		RenderUI(uiComp);*/
	}
	RenderUI();
}

void Menu::Unload()
{
	for (auto entity : View<UICanvas>(registry))
	{
		//Get entity with UI, release components.
		UICanvas* ui = registry.GetComponent<UICanvas>(entity);
		if (ui)
		{
			//MainMenu* exMenu = registry.GetComponent<MainMenu>(entity);
			//exMenu->m_uiCanvas.Release();
			ui->Release();
		}
	}
}
