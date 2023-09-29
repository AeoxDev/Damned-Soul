#include "Menus.h"
#include "SDLHandler.h"
#include <iostream>

using namespace DirectX;

//the Exmenu Folder is meant to be deleted later, do not use it for the real UI images

void SetupMainMenuCanvas(UICanvas& canvas)
{
	canvas.Setup();

	//Position is now (-1, -1) -> (1, 1), where (-1, -1) is the bottom left coorner and (1, 1) is the top right coorner
	canvas.AddImage(ui, "Title", "ExMenu/ExTitle.png", { 0.0f, 0.75f }, { 3.0f, 3.0f });
	canvas.AddImage(ui, "Eye", "ExMenu/Eye.png", { -0.95f, 0.9f }, { 1.5f, 1.5f });
	canvas.AddImage(ui, "Eye2", "ExMenu/Eye.png", { 0.95f, 0.9f }, { 1.5f, 1.5f });
	canvas.AddText(ui, "ExampleText", L"This is the main menu!", { 0.0f, 0.3f }); 
	
	//auto apaa = canvas.m_Images["Eye"].m_Bounds;

	//seperate scope for readability reasons, One scope per button 
	// StartButton
	{
		auto OnClick = [canvas]()
			{
				std::cout << "Click" << std::endl;
			};

		auto OnHover = [canvas]()
			{
				std::cout << "Hover" << std::endl;
				//Change hover button, spawn particles, bla bla 
			};

		canvas.AddButton(ui, "Exmenu/StartButton.png", "", L"", OnClick, OnHover, { 0.0f, -0.2f });
	}

	//Settings Button
	{
		auto OnClick = [canvas]()
			{
				std::cout << "Second" << std::endl;
			};

		auto OnHover = [canvas]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		canvas.AddButton(ui, "Exmenu/OptionsButton.png", "", L"", OnClick, OnHover, { 0.0f, -0.4f });
	}

	//Exit Button
	{
		auto OnClick = [canvas]()
			{
				std::cout << "Second" << std::endl;
			};

		auto OnHover = [canvas]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		canvas.AddButton(ui, "Exmenu/ExitButton.png", "", L"", OnClick, OnHover, { 0.0f, -0.6f });
	}
}

void SetupSettingsCanvas(UICanvas& canvas)
{
	canvas.Setup();

	canvas.AddImage(ui, "Title", "ExMenu/OptionsButtonHover.png", {0.0f, 0.8f }, { 2.0f, 2.0f });
	canvas.AddImage(ui, "Pic1", "ExMenu/D0.png", { -0.95, 0.9f }, { 1.5f, 1.5f });
	canvas.AddImage(ui, "Pic2", "ExMenu/D0.png", { 0.95, 0.9f }, { 1.5f, 1.5f });
	canvas.AddText(ui, "ExampleText", L"This is the settings menu!", { 0.0f, 0.3f });

	canvas.AddText(ui, "Fullscreen", L"Fullscreen:", { -0.1f, -0.2f });
	canvas.AddImage(ui, "FullscreenBox", "ExMenu/CheckBoxBase.png", { 0.1f, -0.2f });
	canvas.AddText(ui, "Volume", L"Volume: 100", { 0.0f, -0.4f });
	canvas.AddText(ui, "Graphics", L"Graphics: Flawless", { 0.0f, -0.6f});

	//seperate scope for readability reasons, One scope per button 
	//Back Button
	{
		auto OnClick = [canvas]()
			{
				std::cout << "Second" << std::endl;
			};

		auto OnHover = [canvas]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		canvas.AddButton(ui, "Exmenu/BackButton.png", "BackButton", L"", OnClick, OnHover, { 0.0f, -0.8f });
	}
}

void SetupHUDCanvas(UICanvas& canvas)
{
	canvas.Setup();

	canvas.AddImage(ui, "EmptyHealth", "ExMenu/EmptyHealth.png", { -0.8f, 0.9f }, { 1.0f, 1.0f });
	canvas.AddImage(ui, "FullHealth", "ExMenu/FullHealth.png", { -0.8f, 0.9f }, { 0.7f, 1.0f });
	canvas.AddText(ui, "Souls", L"Current Souls: 0", { -0.8f, 0.8f });
	canvas.AddText(ui, "ExampleText", L"This is the HUD!", {0.0f, 0.3f });
	canvas.AddText(ui, "Escape", L"Press ECS to return to main menu!", { 0.0f, 0.8f });
	canvas.AddText(ui, "Move", L"Use WASD to move doggo!", { 0.0f, 0.6f });
}