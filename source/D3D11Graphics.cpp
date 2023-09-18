#include "D3D11Graphics.h"
#include "SDLHandler.h"
#include "MemLib/MemLib.hpp"
#include <iostream>
#include <fstream>
#include <string>

//#ifdef _DEBUG
//#include <dxgidebug.h>
//#endif

// Extern
D3D11Data* d3d11Data;
TextureHolder* txHolder;
SamplerStateHolder* smpHolder;
PixelShaderHolder* pixHolder;
VertexShaderHolder* vrtHolder;
ComputeShaderHolder* comHolder;
BufferHolder* bfrHolder;
ViewPortHolder* vpHolder;
RTVHolder* rtvHolder;
DSVHolder* dsvHolder;
SRVHolder* srvHolder;
UAVHolder* uavHolder;
RasterizerHolder* rsHolder;

bool CreateDeviceAndSwapChain(HWND& window, UINT width, UINT height)
{
	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

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
	txHolder = (TextureHolder*)MemLib::spush(sizeof(TextureHolder));
	smpHolder = (SamplerStateHolder*)MemLib::spush(sizeof(SamplerStateHolder));
	pixHolder = (PixelShaderHolder*)MemLib::spush(sizeof(PixelShaderHolder));
	vrtHolder = (VertexShaderHolder*)MemLib::spush(sizeof(VertexShaderHolder));
	comHolder = (ComputeShaderHolder*)MemLib::spush(sizeof(ComputeShaderHolder));
	bfrHolder = (BufferHolder*)MemLib::spush(sizeof(BufferHolder));
	vpHolder = (ViewPortHolder*)MemLib::spush(sizeof(ViewPortHolder));
	rtvHolder = (RTVHolder*)MemLib::spush(sizeof(RTVHolder));
	dsvHolder = (DSVHolder*)MemLib::spush(sizeof(DSVHolder));
	srvHolder = (SRVHolder*)MemLib::spush(sizeof(SRVHolder));
	uavHolder = (UAVHolder*)MemLib::spush(sizeof(UAVHolder));
	rsHolder = (RasterizerHolder*)MemLib::spush(sizeof(RasterizerHolder));
	smpHolder = (SamplerStateHolder*)MemLib::spush(sizeof(SamplerStateHolder));

	if (false == CreateDeviceAndSwapChain(w, sdl.WIDTH, sdl.HEIGHT))
		FAIL_MSG

	return 0;
}

void EndDirectX()
{
	// Release all pixel shaders
	for (int i = 0; i < pixHolder->currentCount; ++i)
		pixHolder->ps_arr[i]->Release();

	// Release all samplers
	for (int i = 0; i < smpHolder->currentCount; ++i)
		smpHolder->smp_arr[i]->Release();

	// Release all textures and associated resources
	for (int i = 0; i < txHolder->currentCount; ++i)
	{
		txHolder->img_arr[i].Release();
		txHolder->srv_arr[i]->Release();
		txHolder->tx_arr[i]->Release();
	}

	// Release all vertex shaders and their input layouts
	for (int i = 0; i < vrtHolder->currentCount; ++i)
	{
		vrtHolder->vs_arr[i]->Release();
		vrtHolder->il_arr[i]->Release();
	}

	// Release all compute shaders
	for (int i = 0; i < comHolder->currentCount; ++i)
	{
		comHolder->cs_arr[i]->Release();
	}

	// Release all buffers shaders
	for (int i = 0; i < bfrHolder->currentCount; ++i)
		bfrHolder->buff_arr[i]->Release();

	// Release all render target views
	for (int i = 0; i < rtvHolder->currentCount; ++i)
		rtvHolder->rtv_arr[i]->Release();
	
	// Release all depth stencil views
	for (int i = 0; i < dsvHolder->currentCount; ++i)
	{
		dsvHolder->dsv_arr[i]->Release();
		dsvHolder->ds_arr[i]->Release();
	}

	// Release all shader resource views
	for (int i = 0; i < srvHolder->currentCount; ++i)
	{
		srvHolder->srv_arr[i]->Release();
		if (srvHolder->srv_resource_arr[i])
			srvHolder->srv_resource_arr[i]->Release();
	}

	// Release all unorderd access views
	for (int i = 0; i < uavHolder->currentCount; ++i)
	{
		uavHolder->uav_arr[i]->Release();
		uavHolder->uav_resource_arr[i]->Release();
	}

	// Release all rasterizer states
	for (int i = 0; i < rsHolder->currentCount; ++i)
		rsHolder->rs_arr[i]->Release();

	// Clear and flush device context
	d3d11Data->deviceContext->ClearState();
	d3d11Data->deviceContext->Flush();

#ifdef _DEBUG
	ID3D11Debug* debugInterface;
	HRESULT hr = d3d11Data->device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugInterface));

	debugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	debugInterface->Release();
#endif

	// Release basic things
	d3d11Data->swapChain->Release();
	d3d11Data->deviceContext->Release();
	d3d11Data->device->Release();
}
