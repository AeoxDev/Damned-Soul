#pragma once
#include "UIRenderer.h"
#include "SDLHandler.h"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"
#include <iostream>
#include "MemLib/PoolPointer.hpp"
#include "EntityFramework.h"
#include "GameRenderer.h"

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
	ID3D11ShaderResourceView* nullsrv = nullptr;
	d3d11Data->deviceContext->PSSetShaderResources(0, 1, &nullsrv);

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

	if (!SetVertexShader(renderStates[ui.RenderSlot].vertexShaders[0]))
	{
		std::cout << "Failed to set UI Vertex Shader!" << std::endl;
		return;
	}
	if (!SetPixelShader(renderStates[ui.RenderSlot].pixelShaders[0]))
	{
		std::cout << "Failed to set UI Pixel Shader!" << std::endl;
		return;
	}
	if (!SetVertexBuffer(renderStates[ui.RenderSlot].vertexBuffer))
	{
		std::cout << "Failed to set UI Vertex Buffer!" << std::endl;
		return;
	}
	if (!SetIndexBuffer(renderStates[ui.RenderSlot].indexBuffer))
	{
		std::cout << "Failed to set UI Index Buffer!" << std::endl;
		return;
	}
	if (!SetRenderTargetViewAndDepthStencil(renderStates[1].renderTargetView, renderStates[1].depthStencilView))
	{
		std::cout << "Failed to set rtv or dsv!" << std::endl;
		return;
	}
	//UnloadShaderResourceView(renderStates[ui.RenderSlot].shaderResourceView);
	if (!SetShaderResourceView(renderStates[ui.RenderSlot].shaderResourceView))
	{
		std::cout << "Failed to set srv!" << std::endl;
		return;
	}

	//SetShaderResourceView(renderStates[ui.RenderSlot].shaderResourceView);

	//d3d11Data->deviceContext->PSSetShaderResources(0, 1, &srvHolder->srv_arr[renderStates[ui.RenderSlot].shaderResourceView]);
	d3d11Data->deviceContext->DrawIndexed(6, 0, 0);
}