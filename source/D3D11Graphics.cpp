#include "D3D11Graphics.h"
#include "SDLHandler.h"
#include "MemLib/MemLib.hpp"
#include <iostream>
#include <fstream>
#include <string>


// Extern
D3D11Data* d3d11Data;
PixelShaderHolder* pixHolder;
VertexShaderHolder* vrtHolder;
BufferHolder* bfrHolder;

////Externs:
//ID3D11DeviceContext* deviceContext;
//ID3D11Device* device;
//IDXGISwapChain* swapChain;

//Privates:
//ID3D11PixelShader* tps; // Test Pixel Shader
//ID3D11VertexShader* tvs; // Test Vertex Shader
//ID3D11InputLayout* til; // Test Input Layout
//ID3D11Texture2D* tbb; // Test Back Buffer
//ID3D11Texture2D* tdst; // Test Depth Stencil Texture
//ID3D11DepthStencilView* tdsv; // Test Depth Stencil View
//ID3D11RenderTargetView* trtv; // Test Render Target View

bool CreateDeviceAndSwapChain(HWND& window, UINT width, UINT height)
{
	UINT flags = 0x0;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &d3d11Data->swapChain, &d3d11Data->device, NULL, &d3d11Data->deviceContext);

	if (FAILED(hr))
	{
		std::cout << "Failed to create device and swap chain!" << std::endl;
		return false;
	}

	return true;
}


#define FAIL_MSG { std::cerr << "Failed to set up D3D11!" << std::endl; return -1; }

int SetupDirectX(HWND& w)
{
	d3d11Data = (D3D11Data*)MemLib::spush(sizeof(D3D11Data));
	pixHolder = (PixelShaderHolder*)MemLib::spush(sizeof(PixelShaderHolder));
	vrtHolder = (VertexShaderHolder*)MemLib::spush(sizeof(VertexShaderHolder));
	bfrHolder = (BufferHolder*)MemLib::spush(sizeof(BufferHolder));

	if (false == CreateDeviceAndSwapChain(w, sdl.WIDTH, sdl.HEIGHT))
		FAIL_MSG

	return 0;
}

void EndDirectX()
{
	// Release all pixel shaders
	for (int i = 0; i < pixHolder->currentCount; ++i)
		pixHolder->ps_arr[i]->Release();

	// Release all vertex shaders and their input layouts
	for (int i = 0; i < vrtHolder->currentCount; ++i)
	{
		vrtHolder->vs_arr[i]->Release();
		vrtHolder->il_arr[i]->Release();
	}

	// Release all buffers shaders
	for (int i = 0; i < bfrHolder->currentCount; ++i)
		bfrHolder->buff_arr[i]->Release();

	// Release basic things
	d3d11Data->swapChain->Release();
	d3d11Data->deviceContext->Release();
	d3d11Data->device->Release();
}
