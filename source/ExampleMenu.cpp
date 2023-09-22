#include "ExampleMenu.h"
#include "D3D11Graphics.h"
#include <filesystem>
#include "SDLHandler.h"
#include "MemLib/MemLib.hpp"
#include <iostream>

using namespace DirectX;

void ExMenu::Update(float dt) //Logic for updating current pages and 
{
	//keyboard and mouse logic
}

ExMenu::ExMenu()
{
}

void ExMenu::Setup(UI& ui)
{
	*(this) = ExMenu();
	m_uiCanvas.Setup();
	
	//std::map<const std::string, UIText> m_Texts;

	//
	m_uiCanvas.AddImage(ui, "Title", "ExMenu/ExTitle.png", { (sdl.WIDTH / 2.0f) - 250.0f, 100.0f }, { 3.0f, 3.0f });
	m_uiCanvas.AddImage(ui, "Eye", "ExMenu/Eye.png", { 50.0f, 50.0f }, { 1.5f, 1.5f });
	m_uiCanvas.AddImage(ui, "Eye2", "ExMenu/Eye.png", { sdl.WIDTH - 68.0f, 50.0f }, { 1.5f, 1.5f });
	m_uiCanvas.AddText(ui, "ExampleText", L"Text!", { sdl.WIDTH / 2.0f - 50.0f, 400.0f }); //text breaks with longer text for unknown reasons :D
	
	//seperate scope for readability reasons, One scope per button 
	// StartButton
	{
		auto OnClick = [this]() 
			{
				std::cout << "First" << std::endl;
			};

		auto OnHover = [this]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		this->m_uiCanvas.AddButton(ui, "Exmenu/StartButton.png", "", L"", OnClick, OnHover, { sdl.WIDTH / 2.0f - 65.0f, 600});
	}

	//Exit Button
	{
		auto OnClick = [this]()
			{
				std::cout << "Second" << std::endl;
			};

		auto OnHover = [this]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		this->m_uiCanvas.AddButton(ui, "Exmenu/ExitButton.png", "", L"", OnClick, OnHover, { sdl.WIDTH / 2.0f - 65.0f, 800 });
	}

	for (auto& button : m_Buttons)
		button.Interact();
}

void ExMenu::operator=(const ExMenu& other)
{
	std::memcpy(this, &other, sizeof(ExMenu));
}

int ExMenu::Run(UI& ui, float dt)
{
	Update(dt);
	this->m_uiCanvas.Render(ui);
	
	return 0;
}
