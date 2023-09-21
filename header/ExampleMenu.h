#pragma once
#include "UICanvas.h"
#include "MemLib\ML_Map.hpp"
#include "MemLib\ML_String.hpp"

class ExMenu : public UICanvas
{
private:
	ML_Map<ML_String, UICanvas> m_Pages;

	PoolPointer<UICanvas> m_CurrentPage;

	UINT activeID = 0;

	void Update(float dt);

	//void Resize();
public:
	ExMenu() = default;
	void Setup(PoolPointer<UI>& ui);

	int Run(PoolPointer<UI>& ui, float dt);

	void operator=(const ExMenu& other);
};