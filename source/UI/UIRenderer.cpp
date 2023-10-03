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

//bool SetupUIRenderer(UIRenderComponents*& inOut)
//{
//	int8_t rtv = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, (RESOURCE_FLAGS)((int)BIND_RENDER_TARGET | (int)BIND_SHADER_RESOURCE), CPU_FLAGS::NONE, sdl.WIDTH, sdl.HEIGHT);
//	inOut->rtv = rtv;
//	ID3D11Texture2D* texture; 
//	GetTextureByType(texture, TEXTURE_HOLDER_TYPE::RENDER_TARGET_VIEW, rtv );
//	//RENDER TARGET
//
//	//UI SURFACE
//	if (FAILED(texture->QueryInterface<IDXGISurface>(&UISurface)))
//	{
//		printf("Failed to create UI Surface\n");
//		return false;
//	}
//
//	//DEPTH STENCIL
//	inOut->dsv = CreateDepthStencil(sdl.WIDTH, sdl.HEIGHT);
//
//	//SHADER RESOURCE VIEW
//	inOut->srv = CreateShaderResourceViewTexture(rtv, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, RESOURCE_FLAGS::BIND_RENDER_TARGET, 0 );
//
//	texture->Release();
//
//	//-----------------------------------------------------------------//
//	struct Vertex {
//		float pos[4];
//		float normal[4];
//		float uv[2];
//	};
//
//	Vertex screenQuad[] =
//	{
//	   { {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },  // TOP RIGHT
//	   { {1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }, // BOTTOM RIGHT
//	   { {-1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }, // TOP LEFT
//	   { {-1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} } // BOTTOM LEFT
//	};
//	inOut->vertexBuffer = CreateVertexBuffer(screenQuad, sizeof(Vertex), ARRAYSIZE(screenQuad), USAGE_IMMUTABLE);
//	uint32_t idxs[] = { 0, 1, 2, 2, 1, 3 };
//	inOut->indexBuffer = CreateIndexBuffer(idxs, sizeof(uint32_t), ARRAYSIZE(idxs));
//	
//	inOut->vertexShader = LoadVertexShader("UIVS.cso", LAYOUT_DESC::SCREEN);
//	inOut->pixelShader = LoadPixelShader("UIPS.cso");
//
//	return true;
//}

void ReleaseUIRenderer()
{
	UISurface->Release();
}

//void UpdateUI( , EntityID& entity)
//{
//	ID3D11ShaderResourceView* nullsrv = nullptr;
//	d3d11Data->deviceContext->PSSetShaderResources(0, 1, &nullsrv);
//
//	ClearDepthStencilView(UIComponents.dsv);
//	ClearRenderTargetView(UIComponents.rtv);
//
//	SetRenderTargetViewAndDepthStencil(UIComponents.rtv, UIComponents.dsv);
//
//	ui->Render(ui);
//}

void Begin2dFrame(UI& ui)
{
	ui.m_RenderTarget->BeginDraw();
	ui.m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void End2dFrame(UI& ui)
{
	ui.m_RenderTarget->EndDraw();
}

void UpdateUI2()
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
void UpdateUI(UICanvas& canvas)
{
	ID3D11ShaderResourceView* nullsrv = nullptr;
	d3d11Data->deviceContext->PSSetShaderResources(0, 1, &nullsrv);

	ClearUI();

	SetRenderTargetViewAndDepthStencil(renderStates[ui.RenderSlot].renderTargetView, renderStates[ui.RenderSlot].depthStencilView);

	ui.Render(canvas);
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