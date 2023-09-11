#pragma once
#include "D3D11Helper.h"
#include "D3D11Graphics.h"


int SetupGameRenderer();
void Render(const int& componentSlot);
void DispatchParticles(UINT threadX, UINT threadY, UINT threadZ);