#pragma once
#include "UICanvas.h"



class ExMenu : public UICanvas
{
private:
	std::map<std::string, PoolPointer<UICanvas>> m_Pages;
	PoolPointer<UICanvas> m_CurrentPage;

	UINT activeID = 0;

	void Render(PoolPointer<UI>& ui);
	void Update(float dt);

	//void Resize();
public:
	ExMenu() = delete;
	ExMenu(PoolPointer<UI>& ui);
	ExMenu(UI*& ui);
	~ExMenu();

	int Run(PoolPointer<UI>& ui, float dt);
};