#include "States_&_Scenes\Menu.h"
#include "UIRenderer.h"
#include "Menus.h"
#include "Hitbox.h"
#include "States_&_Scenes\StateManager.h"
#include "Input.h"

void Menu::ComputeShaders()
{

}

void Menu::Update()
{

}

void Menu::Input()
{
	EntityID ui = registry.entities.at(0).id;
	UICanvas* canvas = registry.GetComponent<UICanvas>(ui);
	//Input controller component.
	if (mouseButtonPressed[MouseButton::left] == released && canvas->m_Buttons[0].m_uiComponent.Intersect({mouseX, mouseY}))
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
	/*for (auto entity : View<UIRenderComponents>(registry))
	{
		UIRenderComponents* uiComp = registry.GetComponent<UIRenderComponents>(entity);
		RenderUI(uiComp);
	}*/
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
