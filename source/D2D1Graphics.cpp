#pragma once
#include "D2D1Graphics.h"
#include "SDLHandler.h"
#include "D3D11Graphics.h"

#include <iostream>


IDXGISurface* UISurface;
/*
D2D1Graphics::D2D1Graphics()
{
	HRESULT hr;

	//TEXTURE 2D
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;							// 32 BIT PER CHANNEL, FLOAT VALUES
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;	//BIND AS RENDER TARGET & SHADER RESOURCE
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
		return;
	}

	//RENDER TARGETS
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; //WE WANT TO ACCESS DATA AS A TEXTURE 2D
	rtvDesc.Format = textureDesc.Format;

	hr = d3d11Data->device->CreateRenderTargetView(texture, &rtvDesc, &UIrtv);
	if FAILED(hr)
	{
		std::cout << "Failed to create UI render target" << std::endl;
		return;
	}

	//SHADER RESOURCE VIEW
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	//WE WANT TO ACCESS DATA AS A TEXTURE 2D
	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MostDetailedMip = 0;					//ID
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;	//DEFAULTS TO -1

	hr = d3d11Data->device->CreateShaderResourceView(texture, &srvDesc, &UIsrv);
	if FAILED(hr)
	{
		std::cout << "Failed to create UI shader resource view" << std::endl;
		return;
	}
	
	texture->Release();
	
	//SHADERS
	std::string byteCode;
	LoadVertexShader(UIVertexShader, vs_path, byteCode);
	LoadPixelShader(UIPixelShader, ps_path);

	//INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = Graphics::GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INPUT LAYOUT");
		return;
	}

	//SCREEN QUAD
	DeferredVertex screenQuad[] =
	{
	   { {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },  // TOP RIGHT

	   { {1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} }, // BOTTOM RIGHT

	   { {-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f} }, // TOP LEFT

	   { {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} } // BOTTOM LEFT
	};
	CreateVertexBuffer(screenQuadBuffer, sizeof(DeferredVertex), sizeof(DeferredVertex) * ARRAYSIZE(screenQuad), screenQuad);

	//BUFFERS
	CreateBuffer(light_buf, sizeof(DirectionalLight::Data));
	CreateBuffer(camera_buf);
	CreateBuffer(numLights_buf);
	CreateBuffer(globalAmbient_buf);
	CreateStructuredBuffer(lights_buf, lights_buf_srv, sizeof(PointLight), MAX_LIGHTS * sizeof(PointLight));
}

D2D1Graphics::~D2D1Graphics()
{

}

IDXGISurface* D2D1Graphics::GetSurface()
{
	return UISurface;
}
*/