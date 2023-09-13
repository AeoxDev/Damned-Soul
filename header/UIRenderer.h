#pragma once
#include <dxgi.h>
#include "D3D11Helper.h"
#include "UI.h"

struct UIRenderComponents
{
	RTV_IDX rtv;
	SRV_IDX srv;
	DSV_IDX dsv;
};

extern IDXGISurface* UISurface;
extern UIRenderComponents UIComponents;

bool SetupUIRenderer();
void RenderUI(UI* ui);
void RenderUI(PoolPointer<UI> ui);
