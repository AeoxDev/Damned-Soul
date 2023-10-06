#pragma once
#include "UI.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"

struct RenderSetupComponent
{
	RS_IDX	rasterizerState = -1;
	PS_IDX	pixelShaders[2] = { -1, -1 };
	VS_IDX	vertexShaders[2] = { -1, -1 };
	GS_IDX	geometryShader = -1;
	CS_IDX	computeShader = -1;
	VB_IDX	vertexBuffer = -1;
	IB_IDX	indexBuffer = -1;
	VP_IDX	viewPort = -1;
	RTV_IDX renderTargetView = -1;
	DSV_IDX depthStencilView = -1;
	SRV_IDX shaderResourceView = -1;
	UAV_IDX unorderedAccessView = -1;
	SMP_IDX samplerState = -1;
	CB_IDX constantBuffer = -1;
};

extern RenderSetupComponent renderStates[8];

extern int backBufferRenderSlot;

bool Setup3dGraphics();
int SetupUIRenderState();
int SetupGameRenderer();
int SetupParticles();

void PrepareDefaultPipeline();

void PrepareBackBuffer(const bool use_skeleton = false);
void ClearBackBuffer();
void ResetGraphicsPipeline();
void Clear(const int& componentSlot);
void RenderIndexed(const size_t& count);
// Calls Draw, instead of DrawIndexed
void RenderOffset(const size_t& count, const size_t& offset);
void ClearParticles();
void Present();
void Dispatch(UINT threadX, UINT threadY, UINT threadZ);