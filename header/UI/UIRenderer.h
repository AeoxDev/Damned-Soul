#pragma once
#include <dxgi.h>
#include "D3D11Helper.h"
#include "EntityFramework.h"
#include "UI.h"

extern IDXGISurface* UISurface;

//bool SetupUIRenderer(UIRenderComponents*& inOut);
void ReleaseUIRenderer();
void UpdateUI();
void RenderUI();

void Begin2dFrame(UI& ui);
void End2dFrame(UI& ui);

void ClearUI();