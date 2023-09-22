#pragma once
#include "UI.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"

struct RenderSetupComponent
{
	RS_IDX	rasterizerState;
	PS_IDX	pixelShader;
	VS_IDX	vertexShader;
	CS_IDX	computeShader;
	VB_IDX	vertexBuffer;
	IB_IDX	indexBuffer;
	VP_IDX	viewPort;
	RTV_IDX renderTargetView;
	DSV_IDX depthStencilView;
	SRV_IDX shaderResourceView;
	UAV_IDX unorderedAccessView;
	SMP_IDX samplerState;
};
extern RenderSetupComponent renderStates[8];
extern int backBufferRenderSlot;

bool Setup3dGraphics();
int SetupUIRenderState();
int SetupGameRenderer();

void PrepareBackBuffer();
void Clear(const int& componentSlot);
void RenderIndexed(const size_t& count);
void ClearParticles();
void Present();
void Dispatch(UINT threadX, UINT threadY, UINT threadZ);