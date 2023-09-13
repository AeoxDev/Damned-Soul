#pragma once
#include "UICanvas.h"

class ExMenu : public UICanvas
{
private:
	std::map<std::string, PoolPointer<UICanvas>> m_Pages;

	PoolPointer<UICanvas> m_CurrentPage;

	UINT activeID = 0;

	void Update(float dt);

	//void Resize();
public:
	ExMenu() = default;// = delete;
	//ExMenu(PoolPointer<UI>& ui);
	void Initialize(PoolPointer<UI>& ui);
	//ExMenu(UI*& ui);
	//~ExMenu();

	int Run(PoolPointer<UI>& ui, float dt);

	void operator=(const ExMenu& other);
};