#pragma once
#include "UI/UIRenderer.h"
#include "SDLHandler.h"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"
#include "MemLib/PoolPointer.hpp"
#include "EntityFramework.h"
#include "GameRenderer.h"
#include <assert.h>
#include <d2d1.h>

IDXGISurface* UISurface;

void ReleaseUIRenderer()
{
	UISurface->Release();
}


void Begin2dFrame(UI& ui)
{
	ui.m_RenderTarget->BeginDraw();
	ui.m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void End2dFrame(UI& ui)
{
	ui.m_RenderTarget->EndDraw();
}

void UpdateUI()
{
	UnsetShaderResourceView(BIND_PIXEL, 0);

	ClearDepthStencilView(renderStates[ui.RenderSlot].depthStencilView);
	ClearRenderTargetView(renderStates[ui.RenderSlot].renderTargetView);

	SetRenderTargetViewAndDepthStencil(renderStates[ui.RenderSlot].renderTargetView, renderStates[ui.RenderSlot].depthStencilView);
}
void ClearUI()
{
	ClearDepthStencilView(renderStates[ui.RenderSlot].depthStencilView);
	ClearRenderTargetView(renderStates[ui.RenderSlot].renderTargetView);
}

void RenderUI()//Render what is drawn to rendertarget.
{

	assert(SetVertexShader(renderStates[ui.RenderSlot].vertexShaders[0]));
	assert(SetPixelShader(renderStates[ui.RenderSlot].pixelShaders[0]));
	
	assert(SetVertexBuffer(renderStates[ui.RenderSlot].vertexBuffer));
	assert(SetIndexBuffer(renderStates[ui.RenderSlot].indexBuffer));
	assert(SetRenderTargetViewAndDepthStencil(renderStates[1].renderTargetView, renderStates[1].depthStencilView));
	assert(SetShaderResourceView(renderStates[ui.RenderSlot].shaderResourceView, BIND_PIXEL, 0));
	d3d11Data->deviceContext->DrawIndexed(6, 0, 0);
}