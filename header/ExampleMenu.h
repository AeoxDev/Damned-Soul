#pragma once
#include "UICanvas.h"
#include "MemLib\ML_Map.hpp"
#include "MemLib\ML_String.hpp"

struct ExMenu
{
private:
	ML_Map<ML_String, UICanvas> m_Pages;
	UICanvas* m_uiCanvas;

	PoolMap<std::string, UICanvas> m_Pages;

	UICanvas m_CurrentPage;

	UINT activeID = 0;

	void Update(float dt);

	//void Resize();
	ExMenu();

	void Setup(UI& ui);

	int Run(UI& ui, float dt);

	void operator=(const ExMenu& other);
};