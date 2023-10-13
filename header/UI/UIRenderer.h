#pragma once
#include "UI.h"

struct IDXGISurface;
extern IDXGISurface* UISurface;

//bool SetupUIRenderer(UIRenderComponents*& inOut);
void ReleaseUIRenderer();
void UpdateUI();
void RenderUI();

void Begin2dFrame(UI& ui);
void End2dFrame(UI& ui);

void ClearUI();

void RedrawUI();