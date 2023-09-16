#pragma once
#include "UIRenderer.h"
#include "SDLHandler.h"
#include "D3D11Graphics.h"
#include <iostream>
#include "MemLib/PoolPointer.hpp"

IDXGISurface* UISurface;
UIRenderComponents UIComponents;

bool SetupUIRenderer()
{
	HRESULT hr;

	//TEXTURE 2D
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;							// 32 BIT PER CHANNEL, FLOAT VALUES
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;	// BIND AS RENDER TARGET & SHADER RESOURCE
	textureDesc.Usage = D3D11_USAGE_DEFAULT;										// GPU READ/WRITE
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.Width = sdl.WIDTH;
	textureDesc.Height = sdl.HEIGHT;

	ID3D11Texture2D* texture = nullptr;
	
	hr = d3d11Data->device->CreateTexture2D(&textureDesc, nullptr, &texture);
	if FAILED(hr)
	{
		std::cout << "Failed to create Texture 2D" << std::endl;
		return false;
	}

	//RENDER TARGET
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; //WE WANT TO ACCESS DATA AS A TEXTURE 2D
	rtvDesc.Format = textureDesc.Format;
	hr = d3d11Data->device->CreateRenderTargetView(texture, &rtvDesc, &(rtvHolder->rtv_arr[rtvHolder->currentCount]));
	if FAILED(hr)
	{
		std::cout << "Failed to create Render Target View" << std::endl;
		return false;
	}
	UIComponents.rtv = (rtvHolder->currentCount)++;

	//UI SURFACE
	if (FAILED(texture->QueryInterface<IDXGISurface>(&UISurface)))
	{
		printf("Failed to create UI Surface\n");
		return false;
	}

	//DEPTH STENCIL
	UIComponents.dsv = CreateDepthStencil(sdl.WIDTH, sdl.HEIGHT);

	//SHADER RESOURCE VIEW
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	//WE WANT TO ACCESS DATA AS A TEXTURE 2D
	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MostDetailedMip = 0;					//ID
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;	//DEFAULTS TO -1

	hr = d3d11Data->device->CreateShaderResourceView(texture, &srvDesc, &srvHolder->srv_arr[srvHolder->currentCount]);
	if (FAILED(hr))
	{
		std::cout << "Failed to create Shader Resource View" << std::endl;
		return false;
	}
	UIComponents.srv = (srvHolder->currentCount)++;

	texture->Release();

	//-----------------------------------------------------------------//
	struct Vertex {
		float pos[4];
		float normal[4];
		float uv[2];
	};

	Vertex screenQuad[] =
	{
	   { {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },  // TOP RIGHT
	   { {1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }, // BOTTOM RIGHT
	   { {-1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} }, // TOP LEFT
	   { {-1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} } // BOTTOM LEFT
	};
	UIComponents.vertexBuffer = CreateVertexBuffer(screenQuad, sizeof(Vertex), ARRAYSIZE(screenQuad));
	uint32_t idxs[] = { 0, 1, 2, 2, 1, 3 };
	UIComponents.indexBuffer = CreateIndexBuffer(idxs, sizeof(uint32_t), ARRAYSIZE(idxs));
	
	UIComponents.vertexShader = LoadVertexShader("UIVS.cso", LAYOUT_DESC::SCREEN);
	UIComponents.pixelShader = LoadPixelShader("UIPS.cso");

	return true;
}

void UpdateUI(PoolPointer<UI> ui)
{
	ID3D11ShaderResourceView* nullsrv = nullptr;
	d3d11Data->deviceContext->PSSetShaderResources(0, 1, &nullsrv);

	ClearDepthStencilView(UIComponents.dsv);
	ClearRenderTargetView(UIComponents.rtv);

	SetRenderTargetViewAndDepthStencil(UIComponents.rtv, UIComponents.dsv);

	ui->Render(ui);
}

void RenderUI()
{
	if (!SetVertexShader(UIComponents.vertexShader))
	{
		std::cout << "Failed to set UI Vertex Shader!" << std::endl;
		return;
	}
	if (!SetPixelShader(UIComponents.pixelShader))
	{
		std::cout << "Failed to set UI Pixel Shader!" << std::endl;
		return;
	}
	if (!SetVertexBuffer(UIComponents.vertexBuffer))
	{
		std::cout << "Failed to set UI Vertex Buffer!" << std::endl;
		return;
	}
	if (!SetIndexBuffer(UIComponents.indexBuffer))
	{
		std::cout << "Failed to set UI Index Buffer!" << std::endl;
		return;
	}


	d3d11Data->deviceContext->PSSetShaderResources(0, 1, &srvHolder->srv_arr[UIComponents.srv]);
	
	d3d11Data->deviceContext->DrawIndexed(6, 0, 0);
}