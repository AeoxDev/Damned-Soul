#pragma once
#include "UICanvas.h"
#include "PoolUtility.hpp"

class ExMenu : public UICanvas
{
private:
	PoolMap<std::string, UICanvas> m_Pages;

	PoolPointer<UICanvas> m_CurrentPage;

	UINT activeID = 0;

	void Update(float dt);

	//void Resize();
public:
	ExMenu() = default;
	void Initialize(PoolPointer<UI>& ui);
	void Release();

	int Run(PoolPointer<UI>& ui, float dt);

	void operator=(const ExMenu& other);
};