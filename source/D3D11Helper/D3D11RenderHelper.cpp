#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include <iostream>

VP_IDX CreateViewport(const size_t& width, const size_t& height)
{
	D3D11_VIEWPORT* vp = &vpHolder->vp_arr[vpHolder->currentCount];
	vp->TopLeftX = 0;
	vp->TopLeftY = 0;
	vp->Width = static_cast<float>(width);
	vp->Height = static_cast<float>(height);
	vp->MinDepth = 0;
	vp->MaxDepth = 1;

	return (vpHolder->currentCount)++;
}

bool SetViewport(const VP_IDX idx)
{
	if (vpHolder->currentCount < idx || idx < 0)
	{
		std::cerr << "Viewport index out of range!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->RSSetViewports(1, &(vpHolder->vp_arr[idx]));
	return true;
}


RTV_IDX CreateRenderTargetView()
{
	// get the address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(d3d11Data->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}

	//D3D11_RENDER_TARGET_VIEW_DESC desc;
	//desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//desc.Texture2D.MipSlice = 0;

	// use the back buffer address to create the render target
	// null as description to base it on the backbuffers values
	HRESULT hr = d3d11Data->device->CreateRenderTargetView(backBuffer, NULL, &(rtvHolder->rtv_arr[rtvHolder->currentCount]));
	backBuffer->Release();

	if (FAILED(hr))
	{
		std::cerr << "Failed to create Render Target View!" << std::endl;
		return false;
	}

	return (rtvHolder->currentCount)++;

}

DSV_IDX CreateDepthStencil(const size_t& width, const size_t& height)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = d3d11Data->device->CreateTexture2D(&textureDesc, nullptr, &(dsvHolder->ds_arr[dsvHolder->currentCount]));
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Depth Stencil Texture!" << std::endl;
		return false;
	}

	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//depthStencilDesc.DepthEnable = true;
	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;

	hr = d3d11Data->device->CreateDepthStencilView(dsvHolder->ds_arr[dsvHolder->currentCount], 0, &(dsvHolder->dsv_arr[dsvHolder->currentCount]));
	if (FAILED(hr))
	{
		dsvHolder->ds_arr[dsvHolder->currentCount]->Release(); // Release if failed
		std::cerr << "Failed to create Depth Stencil View!" << std::endl;
		return false;
	}

	return (dsvHolder->currentCount)++;
}

bool SetRenderTargetViewAndDepthStencil(const RTV_IDX idx_rtv, const DSV_IDX idx_dsv)
{
	if (rtvHolder->currentCount < idx_rtv || idx_rtv < 0 || dsvHolder->currentCount < idx_dsv || idx_dsv < 0)
	{
		std::cerr << "Failed to set render target view and depth stencil view, out of range!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->OMSetRenderTargets(1, &(rtvHolder->rtv_arr[idx_rtv]), dsvHolder->dsv_arr[idx_dsv]);
	return true;
}


void ClearRenderTargetView(const RTV_IDX idx)
{
	float color[4] = { 1.0f, 0.0f, 0.84f, 1.0f };
	d3d11Data->deviceContext->ClearRenderTargetView(rtvHolder->rtv_arr[idx], color);
}

void ClearDepthStencilView(const DSV_IDX idx)
{
	d3d11Data->deviceContext->ClearDepthStencilView(dsvHolder->dsv_arr[idx], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}


RS_IDX CreateRasterizerState(const bool cull, const bool solid)
{
	D3D11_RASTERIZER_DESC desc;
	desc.FillMode = solid ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
	desc.CullMode = cull ? D3D11_CULL_BACK : D3D11_CULL_NONE;
	desc.FrontCounterClockwise = false;
	desc.DepthBias = 0.0f;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = false;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = true; // always multisample kek
	desc.AntialiasedLineEnable = true;

	HRESULT hr = d3d11Data->device->CreateRasterizerState(&desc, &(rsHolder->rs_arr[rsHolder->currentCount]));
	if (FAILED(hr))
	{
		std::cerr << "Failed to create rasterizer state!" << std::endl;
		return -1;
	}

	return (rsHolder->currentCount)++;
}

bool SetRasterizerState(const RS_IDX idx)
{
	if (idx < 0 || rsHolder->currentCount < idx)
	{
		std::cerr << "Rasterizer state out of range!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->RSSetState(rsHolder->rs_arr[idx]);
	return true;
}