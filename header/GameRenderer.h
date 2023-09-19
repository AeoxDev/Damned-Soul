#pragma once
#include "UI.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"


int SetupGameRenderer();

void Clear(const int& componentSlot);
void Render(const size_t& count);
void Present();
void Dispatch(UINT threadX, UINT threadY, UINT threadZ);