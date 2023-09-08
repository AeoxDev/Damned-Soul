#include "ExampleMenu.h"
#include "D3D11Graphics.h"
#include <filesystem>

using namespace DirectX::SimpleMath;

float scalingFactor;
Vector2 center;

void ExMenu::Render(PoolPointer<UI>& ui)
{
	//deviceContext->ClearRenderTargetView()
}

void ExMenu::Update(float dt) //Logic for updating current pages and 
{
	//keyboard and mouse logic
}

ExMenu::ExMenu(PoolPointer<UI>& ui)
{
	/*const Vector2 buttonPositions[] = {				//an array of positions for all the buttons
		{center.x * 1.75f, center.y * 1.85f},
		{center.x * 0.75f, center.y * 1.85f},
		{center.x * 1.25f, center.y * 1.85f},
		{center.x * 0.25f, center.y * 1.85f}
	};*/

	AddImage(ui, "Title", "ExMenu/ExTitle.png", {100,100 }, { 1.0f, 1.0f });
	AddImage(ui, "Eye", "ExMenu/Eye.png", { 50, 50}, { 1.5f, 1.5f});

}

ExMenu::~ExMenu()
{
	for (auto& [name, page] : m_Pages)
	{
		MemLib::pfree(page);
	}
}

int ExMenu::Run(PoolPointer<UI>& ui, float dt)
{
	Update(dt);
	Render(ui);
	
	return 0;
}
