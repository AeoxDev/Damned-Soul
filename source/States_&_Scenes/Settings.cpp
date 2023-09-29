#include "States_&_Scenes\Settings.h"
#include "States_&_Scenes\StateManager.h"
#include "Input.h"
#include "Menus.h"
#include "UIRenderer.h"


void SettingsState::Setup()
{
	//Setup Settings UI
	EntityID SettingsPage = registry.CreateEntity();

	this->registry.AddComponent<UICanvas>(SettingsPage);
	UICanvas* SettingsCanvas = registry.GetComponent<UICanvas>(SettingsPage);
	SetupSettingsCanvas(*SettingsCanvas);
	UpdateUI(*SettingsCanvas);

	// Create UI and example menu
	//*ui = UI();

	//exMenu->Setup(*ui);
	////ui->SetCurrentCanvas(exMenu->m_CurrentPage);
	//DrawGUI(*mainMenu);
	Begin2dFrame(ui);
	SettingsCanvas->Render(ui);
	End2dFrame(ui);
}

void SettingsState::Clear()
{
}

void SettingsState::ComputeShaders()
{
}

void SettingsState::Render()
{
	RenderUI();
}

void SettingsState::Input()
{
	EntityID ui = registry.entities.at(0).id;
	UICanvas* canvas = registry.GetComponent<UICanvas>(ui);
	//Input controller component.
	if (mouseButtonPressed[MouseButton::left] == released && canvas->m_Buttons[0].m_uiComponent.Intersect({ mouseX, mouseY }))
	{
		SetInMainMenu(true);
		SetInSettings(false);
		stateManager.menu.Setup();
		Unload();
	}
}

void SettingsState::Update()
{
}

void SettingsState::Unload()
{
	for (auto entity : View<UICanvas>(registry))
	{
		//Get entity with UI, release components.
		UICanvas* ui = registry.GetComponent<UICanvas>(entity);
		if (ui)
		{
			ui->Release();
		}
	}
}
