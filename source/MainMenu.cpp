#include "MainMenu.h"
#include "SDLHandler.h"
#include <iostream>

using namespace DirectX;

void SetupMainMenuCanvas(UICanvas& canvas)
{
	canvas.Setup();

	//std::map<const std::string, UIText> m_Texts;

	//
	canvas.AddImage(ui, "Title", "ExMenu/ExTitle.png", { (sdl.WIDTH / 2.0f) - 250.0f, 100.0f }, { 3.0f, 3.0f });
	canvas.AddImage(ui, "Eye", "ExMenu/Eye.png", { 50.0f, 50.0f }, { 1.5f, 1.5f });
	canvas.AddImage(ui, "Eye2", "ExMenu/Eye.png", { sdl.WIDTH - 68.0f, 50.0f }, { 1.5f, 1.5f });
	canvas.AddText(ui, "ExampleText", L"Text!", { sdl.WIDTH / 2.0f - 50.0f, 400.0f }); //text breaks with longer text for unknown reasons :D

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

	for (auto& button : canvas.m_Buttons)
		button.Interact();
}
