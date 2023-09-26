#include "Menus.h"
#include "SDLHandler.h"
#include <iostream>

using namespace DirectX;

void SetupMainMenuCanvas(UICanvas& canvas)
{
	canvas.Setup();

	canvas.AddImage(ui, "Title", "ExMenu/ExTitle.png", { (sdl.WIDTH / 2.0f) - 250.0f, 100.0f }, { 3.0f, 3.0f });
	canvas.AddImage(ui, "Eye", "ExMenu/Eye.png", { 50.0f, 50.0f }, { 1.5f, 1.5f });
	canvas.AddImage(ui, "Eye2", "ExMenu/Eye.png", { sdl.WIDTH - 68.0f, 50.0f }, { 1.5f, 1.5f });
	canvas.AddText(ui, "ExampleText", L"This is the main menu!", { sdl.WIDTH / 2.0f - 50.0f, 300.0f }); 
	
	//seperate scope for readability reasons, One scope per button 
	// StartButton
	{
		auto OnClick = [canvas]()
			{
				std::cout << "First" << std::endl;
			};

		auto OnHover = [canvas]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		canvas.AddButton(ui, "Exmenu/StartButton.png", "", L"", OnClick, OnHover, { sdl.WIDTH / 2.0f - 65.0f, 600 });
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

		canvas.AddButton(ui, "Exmenu/ExitButton.png", "", L"", OnClick, OnHover, { sdl.WIDTH / 2.0f - 65.0f, 800 });
	}
}

void SetupSettingsCanvas(UICanvas& canvas)
{
	canvas.Setup();

	canvas.AddImage(ui, "Title", "ExMenu/P2Character.png", { (sdl.WIDTH / 2.0f) - (426.0f / 8.0f), 100.0f }, { 1.0f, 1.0f });
	canvas.AddImage(ui, "Pic1", "ExMenu/D0.png", { 50.0f, 50.0f }, { 1.5f, 1.5f });
	canvas.AddImage(ui, "Pic2", "ExMenu/D0.png", { sdl.WIDTH - 68.0f, 50.0f }, { 1.5f, 1.5f });
	canvas.AddText(ui, "ExampleText", L"This is the settings menu!", { sdl.WIDTH / 2.0f - 50.0f, 300.0f });



	//seperate scope for readability reasons, One scope per button 
	// StartButton
	{
		auto OnClick = [canvas]()
			{
				std::cout << "First" << std::endl;
			};

		auto OnHover = [canvas]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		canvas.AddButton(ui, "Exmenu/StartButton.png", "", L"", OnClick, OnHover, { sdl.WIDTH / 2.0f - 65.0f, 600 });
	}

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

		canvas.AddButton(ui, "Exmenu/BackButton.png", "BackButton", L"", OnClick, OnHover, { sdl.WIDTH / 2.0f - 65.0f, 800 });
	}
}

void SetupHUDCanvas(UICanvas& canvas)
{
	canvas.Setup();

	canvas.AddImage(ui, "EmptyHealth", "ExMenu/EmptyHealth.png", { 150.0f, 50.0f }, { 1.0f, 1.0f });
	canvas.AddImage(ui, "FullHealth", "ExMenu/FullHealth.png", { 150.0f, 50.0f }, { 0.7f, 1.0f });
	canvas.AddText(ui, "ExampleText", L"This is the HUD!", { sdl.WIDTH / 2.0f - 50.0f, 300.0f });
	canvas.AddText(ui, "Souls", L"Current Souls: 0", { 125.0f, 100.0f });
}