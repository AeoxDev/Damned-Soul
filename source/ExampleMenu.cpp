#include "ExampleMenu.h"
#include "D3D11Graphics.h"
#include <filesystem>

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
	AddImage(ui, "Title", "ExMenu/ExTitle.png", { 100,100 }, { 1.0f, 1.0f });
	AddImage(ui, "Eye", "ExMenu/Eye.png", { 50, 50 }, { 1.5f, 1.5f });
	AddImage(ui, "Eye2", "ExMenu/Eye.png", { 780, 360 }, { 1.5f, 1.5f });
	AddText(ui, "ExampleText", L"This is some example text", { 400, 400 });
	
	//seperate scope for readability reasons, One scope per button 
	{
		auto OnClick = [this]() 
			{
				//Do click stuff
			};

		auto OnHover = [this]()
			{
				//Change hover button, spawn particles, bla bla 
			};

		AddButton(ui, "Exmenu/ExitButton.png", "", L"Hello", OnClick, OnHover, {600, 200});
	}
}

//ExMenu::ExMenu(UI*& ui)
//{
//	AddImage(ui, "Title", "ExMenu/ExTitle.png", { 100,100 }, { 1.0f, 1.0f });
//	AddImage(ui, "Eye", "ExMenu/Eye.png", { 50, 50 }, { 1.5f, 1.5f });
//	AddImage(ui, "Eye2", "ExMenu/Eye.png", { 780, 360 }, { 1.5f, 1.5f });
//
//}

//ExMenu::~ExMenu()
//{
//	for (auto& [name, page] : m_Pages)
//	{
//		MemLib::pfree(page);
//	}
//}

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
