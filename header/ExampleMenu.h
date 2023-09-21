#pragma once
#include "UICanvas.h"
#include "PoolUtility.hpp"

struct ExMenu
{
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