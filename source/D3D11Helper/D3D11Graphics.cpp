#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "D3D11Helper/D3D11Graphics.h"
#include "SDLHandler.h"
#include "MemLib/MemLib.hpp"
#include <fstream>
#include <assert.h>

//#ifdef _DEBUG
//#include <dxgidebug.h>
//#endif

// Extern
D3D11Data* d3d11Data;
TextureHolder* txHolder;
SamplerStateHolder* smpHolder;
PixelShaderHolder* pixHolder;
VertexShaderHolder* vrtHolder;
GeometryShaderHolder* geoHolder;
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
	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;
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

	swapChainDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &d3d11Data->swapChain, &d3d11Data->device, NULL, &d3d11Data->deviceContext);
	assert(SUCCEEDED(hr));

	return SUCCEEDED(hr);
}

#define FAIL_MSG { std::cerr << "Failed to set up D3D11!" << std::endl; return -1; }


int SetupDirectX(HWND& w)
{
#define PUSH_AND_INITIALIZE(pointer, type) { pointer = (type*)MemLib::spush(sizeof(type)); *pointer = type(); }

	PUSH_AND_INITIALIZE(d3d11Data, D3D11Data);
	//d3d11Data = (D3D11Data*)MemLib::spush(sizeof(D3D11Data));
	PUSH_AND_INITIALIZE(txHolder, TextureHolder);
	//txHolder = (TextureHolder*)MemLib::spush(sizeof(TextureHolder));
	PUSH_AND_INITIALIZE(smpHolder, SamplerStateHolder);
	//smpHolder = (SamplerStateHolder*)MemLib::spush(sizeof(SamplerStateHolder));
	PUSH_AND_INITIALIZE(pixHolder, PixelShaderHolder);
	//pixHolder = (PixelShaderHolder*)MemLib::spush(sizeof(PixelShaderHolder));
	PUSH_AND_INITIALIZE(vrtHolder, VertexShaderHolder);
	//vrtHolder = (VertexShaderHolder*)MemLib::spush(sizeof(VertexShaderHolder));
	PUSH_AND_INITIALIZE(comHolder, ComputeShaderHolder);
	//comHolder = (ComputeShaderHolder*)MemLib::spush(sizeof(ComputeShaderHolder));
	PUSH_AND_INITIALIZE(bfrHolder, BufferHolder);
	//bfrHolder = (BufferHolder*)MemLib::spush(sizeof(BufferHolder));
	PUSH_AND_INITIALIZE(vpHolder, ViewPortHolder);
	//vpHolder = (ViewPortHolder*)MemLib::spush(sizeof(ViewPortHolder));
	PUSH_AND_INITIALIZE(rtvHolder, RTVHolder);
	//rtvHolder = (RTVHolder*)MemLib::spush(sizeof(RTVHolder));
	PUSH_AND_INITIALIZE(dsvHolder, DSVHolder);
	//dsvHolder = (DSVHolder*)MemLib::spush(sizeof(DSVHolder));
	PUSH_AND_INITIALIZE(srvHolder, SRVHolder);
	//srvHolder = (SRVHolder*)MemLib::spush(sizeof(SRVHolder));
	PUSH_AND_INITIALIZE(uavHolder, UAVHolder);
	//uavHolder = (UAVHolder*)MemLib::spush(sizeof(UAVHolder));
	PUSH_AND_INITIALIZE(rsHolder, RasterizerHolder);
	//rsHolder = (RasterizerHolder*)MemLib::spush(sizeof(RasterizerHolder));
	PUSH_AND_INITIALIZE(geoHolder, GeometryShaderHolder);
	bool succeded = CreateDeviceAndSwapChain(w, sdl.WIDTH, sdl.HEIGHT);
	assert(true == succeded);
	if (!succeded)
	{
		return -1;
	}
	return 0;
}

void EndDirectX()
{
	// Release all pixel shaders
	for (auto& [key, val] : pixHolder->ps_map)
	{
		if (val != nullptr)
			val->Release();
	}
	pixHolder->ps_map.clear();

	// Release all samplers
	for (auto& [key, val] : smpHolder->smp_map)
	{
		if (val != nullptr)
			val->Release();
	}
	smpHolder->smp_map.clear();

	// Release all vertex shaders and their input layouts
	for (auto& [key, val] : vrtHolder->vs_map)
	{
		if (val != nullptr)
			val->Release();
	}
	vrtHolder->vs_map.clear();

	for (auto& [key, val] : vrtHolder->il_map)
	{
		if (val != nullptr)
			val->Release();
	}
	vrtHolder->il_map.clear();

	// Release all geometry shaders
	for (auto& [key, val] : geoHolder->gs_map)
	{
		if (val != nullptr)
			val->Release();
	}
	geoHolder->gs_map.clear();

	// Release all compute shaders
	for (auto& [key, val] : comHolder->cs_map)
	{
		if (val != nullptr)
			val->Release();
	}
	comHolder->cs_map.clear();

	// Release all textures and associated resources
	//for (auto& [key, val] : txHolder->img_map)
	//	val.Release(); // Is not a pointer
	//txHolder->img_map.clear();

	for (auto& [key, val] : txHolder->srv_map)
	{
		if (val != nullptr)
			val->Release();
	}
	txHolder->srv_map.clear();

	for (auto& [key, val] : txHolder->tx_map)
	{
		if (val != nullptr)
			val->Release();
	}
	txHolder->tx_map.clear();

	// Release all buffers shaders
	for (auto& [key, val] : bfrHolder->buff_map)
	{
		if (val != nullptr)
			val->Release();
	}
	bfrHolder->buff_map.clear();

	// Release all render target views
	for (auto& [key, val] : rtvHolder->rtv_map)
	{
		if (val != nullptr)
			val->Release();
	}
	rtvHolder->rtv_map.clear();

	// Release all RTV textures
	for (auto& [key, val] : rtvHolder->tx_map)
	{
		if (val != nullptr)
			val->Release();
	}
	rtvHolder->tx_map.clear();
	
	// Release all depth stencil views
	for (auto& [key, val] : dsvHolder->dsv_map)
	{
		if (val != nullptr)
			val->Release();
	}
	dsvHolder->dsv_map.clear();

	for (auto& [key, val] : dsvHolder->ds_map)
	{
		if (val != nullptr)
			val->Release();
	}
	dsvHolder->ds_map.clear();

	// Release all shader resource views
	for (auto& [key, val] : srvHolder->srv_resource_map)
	{
		if (val != nullptr)
			val->Release();
	}
	srvHolder->srv_resource_map.clear();

	for (auto& [key, val] : srvHolder->srv_map)
	{
		if (val != nullptr)
			val->Release();
	}
	srvHolder->srv_map.clear();

	// Release all unorderd access views
	for (auto& [key, val] : uavHolder->uav_map)
	{
		if (val != nullptr)
			val->Release();
	}	
	uavHolder->uav_map.clear();

	for (auto& [key, val] : uavHolder->uav_resource_map)
	{
		if (val != nullptr)
			val->Release();
	}
	uavHolder->uav_resource_map.clear();

	// Release all rasterizer states
	for (auto& [key, val] : rsHolder->rs_map)
	{
		if (val != nullptr)
			val->Release();
	}
	rsHolder->rs_map.clear();

	// Clear and flush device context
	d3d11Data->deviceContext->ClearState();
	d3d11Data->deviceContext->Flush();

	// Release basic things
	d3d11Data->swapChain->Release();
	d3d11Data->deviceContext->Release();

#ifdef _DEBUG
	// Check for stuffs
	ID3D11Debug* debugInterface;
	HRESULT hr = d3d11Data->device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugInterface));
#endif

	d3d11Data->device->Release();
#ifdef _DEBUG
	// Check for stuffs
	debugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	debugInterface->Release();
#endif
}
