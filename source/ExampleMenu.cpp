#include "ExampleMenu.h"
#include "D3D11Graphics.h"
#include <filesystem>
#include "SDLHandler.h"
#include "MemLib/MemLib.hpp"

using namespace DirectX;

float scalingFactor;
XMFLOAT2 center;

void ExMenu::Update(float dt) //Logic for updating current pages and 
{
	//keyboard and mouse logic
}

//ExMenu::ExMenu(PoolPointer<UI>& ui)
//{
//	AddImage(ui, "Title", "ExMenu/ExTitle.png", {100,100 }, { 1.0f, 1.0f });
//	AddImage(ui, "Eye", "ExMenu/Eye.png", { 50, 50}, { 1.5f, 1.5f});
//	AddImage(ui, "Eye2", "ExMenu/Eye.png", { 780, 360 }, { 1.5f, 1.5f });
//	//AddText(ui, "ExampleText", L"This is some example text", { 400, 400 });
//	//AddButton(ui, "Exmenu/ExitButton.png")
//}

void ExMenu::Initialize(PoolPointer<UI>& ui)
{
	*(this) = ExMenu();
	AddImage(ui, "Title", "ExMenu/ExTitle.png", { (sdl.WIDTH / 2.0f) - 250.0f, 100.0f }, { 3.0f, 3.0f });
	AddImage(ui, "Eye", "ExMenu/Eye.png", { 50.0f, 50.0f }, { 1.5f, 1.5f });
	AddImage(ui, "Eye2", "ExMenu/Eye.png", { sdl.WIDTH - 68.0f, 50.0f }, { 1.5f, 1.5f });
	AddText(ui, "ExampleText", L"Hello", { sdl.WIDTH / 2.0f - 50.0f, 400.0f }); //text breaks with longer text for unknown reasons :D
	
	
	//seperate scope for readability reasons, One scope per button 
	// StartButton
	{
		auto OnClick = [this]() 
			{
				//Do click stuff
			};

		auto OnHover = [this]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		AddButton(ui, "Exmenu/StartButton.png", "", L"", OnClick, OnHover, { sdl.WIDTH / 2.0f - 65.0f, 600});
	}

	//Exit Button
	{
		auto OnClick = [this]()
			{
				//Do click stuff
			};

		auto OnHover = [this]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		AddButton(ui, "Exmenu/ExitButton.png", "", L"", OnClick, OnHover, { sdl.WIDTH / 2.0f - 65.0f, 800 });
	}
}

void ExMenu::Release()
{
	//auto keys = m_Pages.GetKeys();
	//for (unsigned int i = 0; i < m_Pages.Size(); ++i)
	//	MemLib::pfree(m_Pages[keys[i]]);
}

void ExMenu::operator=(const ExMenu& other)
{
	std::memcpy(this, &other, sizeof(ExMenu));
}

int ExMenu::Run(PoolPointer<UI>& ui, float dt)
{
	Update(dt);
	Render(ui);
	
	return 0;
}
