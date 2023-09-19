#pragma once
#include <dxgi.h>
#include "D3D11Helper.h"
#include "UI.h"

struct UIRenderComponents
{
	RTV_IDX rtv;
	SRV_IDX srv;
	DSV_IDX dsv;
	VS_IDX vertexShader;
	PS_IDX	pixelShader;
	VB_IDX	vertexBuffer;
	IB_IDX	indexBuffer;
};

extern IDXGISurface* UISurface;
extern UIRenderComponents UIComponents;

bool SetupUIRenderer();
void ReleaseUIRenderer();
void UpdateUI(PoolPointer<UI> ui);
void RenderUI();