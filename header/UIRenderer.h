#pragma once
#include <dxgi.h>
#include "D3D11Helper.h"
#include "EntityFramework.h"
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

bool SetupUIRenderer(UIRenderComponents*& inOut);
void ReleaseUIRenderer();
void DrawGUI(Registry& registry, EntityID& entity);
void RenderUI(UIRenderComponents*& uiComp);