#pragma once
#include "UI.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"


int SetupGameRenderer();

//I never wanted this... but this was the only solution...
//Exists solely for particles...
void SetCameraBuffer(CB_IDX index);
void Clear(const int& componentSlot);
void Render(const size_t& count);
void Present();
void DispatchParticles(UINT threadX, UINT threadY, UINT threadZ);
void RenderParticles(const size_t& count);
void ChangeTopology(D3D_PRIMITIVE_TOPOLOGY toplogy);