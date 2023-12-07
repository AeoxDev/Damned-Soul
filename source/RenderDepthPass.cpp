#include "RenderDepthPass.h"
#include "D3D11Helper\D3D11Helper.h"
#include "D3D11Helper\D3D11Graphics.h"
#include "SDLHandler.h"
#include "GameRenderer.h"
#include <assert.h>

PS_IDX depthPassShader;

ID3D11Texture2D* depthPassTexture;
ID3D11DepthStencilView* depthPassDSV;
ID3D11ShaderResourceView* depthPassSRV;

PS_IDX shadowMapPixelShader;

VP_IDX shadowViewport;
DSV_IDX shadowDSV;

ID3D11Texture2D* shadowPassTexture;
ID3D11RenderTargetView* shadowRTV;
ID3D11ShaderResourceView* shadowSRV;


PS_IDX CreateDepthPassPixelShader(char* shader)
{
	depthPassShader = LoadPixelShader(shader);
	return depthPassShader;
}

PS_IDX GetDepthPassPixelShader()
{
	return depthPassShader;
}

void CreateDepthPass()
{
	D3D11_TEXTURE2D_DESC desc = { 0 };
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	// Take the height and width of the loaded image and set it as the dimensions for the texture
	desc.Width = (UINT)sdl.BASE_WIDTH;
	desc.Height = (UINT)sdl.BASE_HEIGHT;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = (D3D11_USAGE)D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = BIND_DEPTH_STENCIL | BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	// Attempt to create a texture in the device
	HRESULT hr = d3d11Data->device->CreateTexture2D(&desc, NULL, &depthPassTexture);
	assert(!FAILED(hr));

	hr = d3d11Data->device->CreateDepthStencilView(depthPassTexture, &dsvDesc, &depthPassDSV);
	assert(!FAILED(hr));

	hr = d3d11Data->device->CreateShaderResourceView(depthPassTexture, &srvDesc, &depthPassSRV);
}

void SetDepthPassTexture(bool forRendering)
{
	if (forRendering)
	{
		float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		SetPixelShader(depthPassShader);
		d3d11Data->deviceContext->ClearDepthStencilView(depthPassDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		ID3D11RenderTargetView* nullRTV = nullptr;
		d3d11Data->deviceContext->OMSetRenderTargets(1, &nullRTV, depthPassDSV);

	}
	else
	{
		d3d11Data->deviceContext->PSSetShaderResources(3, 1, &depthPassSRV);
	}

}

// ARIAN SKREV DETTA, FIGHT ME
void SetDepthPassTextureCompute(bool set)
{
	if (set)
		d3d11Data->deviceContext->CSSetShaderResources(0, 1, &depthPassSRV);
	else
		UnsetShaderResourceView(BIND_COMPUTE, 0);

}

void UnsetDepthPassTexture(bool forRendering)
{
	if (forRendering)
	{
		//SetRenderTargetViewAndDepthStencil(depthPassRTV, renderStates[backBufferRenderSlot].depthStencilView);
	}
	else
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		d3d11Data->deviceContext->PSSetShaderResources(3, 1, &nullSRV);
	}
}

void ReleaseDepthPass()
{
	depthPassTexture->Release();
	depthPassDSV->Release();
	depthPassSRV->Release();
	shadowPassTexture->Release();
	shadowRTV->Release();
	shadowSRV->Release();
}

void CreateShadowMap(const unsigned& width, const unsigned& height)
{
	char shader[] = "ShadowMapPixel.cso";
	shadowMapPixelShader = LoadPixelShader(shader);
	//Create viewport, dsv, texture, rtv, srv.
	shadowViewport = CreateViewport(width, height);
	shadowDSV = CreateDepthStencil(width, height);

	RESOURCE_FLAGS flags = (RESOURCE_FLAGS)((int)BIND_RENDER_TARGET | (int)BIND_SHADER_RESOURCE);
	uint8_t currentIdx = rtvHolder->NextIdx();

	D3D11_TEXTURE2D_DESC desc = { 0 };
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	// Take the height and width of the loaded image and set it as the dimensions for the texture
	desc.Width = (UINT)width;
	desc.Height = (UINT)height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM;//We only need the depth
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = (D3D11_USAGE)D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = BIND_RENDER_TARGET | BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	// Attempt to create a texture in the device
	HRESULT hr = d3d11Data->device->CreateTexture2D(&desc, NULL, &shadowPassTexture);
	assert(!FAILED(hr));

	hr = d3d11Data->device->CreateRenderTargetView(shadowPassTexture, NULL, &shadowRTV);
	assert(!FAILED(hr));

	hr = d3d11Data->device->CreateShaderResourceView(shadowPassTexture, NULL, &shadowSRV);
}

void SetShadowmap(bool forRendering)
{
	if (forRendering)
	{
		float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		SetPixelShader(shadowMapPixelShader);
		d3d11Data->deviceContext->ClearRenderTargetView(shadowRTV, color);
		ClearDepthStencilView(shadowDSV);
		d3d11Data->deviceContext->OMSetRenderTargets(1, &shadowRTV, dsvHolder->dsv_map[shadowDSV]);
		SetViewport(shadowViewport);
		
	}
	else
	{
		d3d11Data->deviceContext->PSSetShaderResources(4, 1, &shadowSRV);
	}
}

void UnsetShadowmap(bool forRendering)
{
	if (forRendering)
	{
		//SetRenderTargetViewAndDepthStencil(depthPassRTV, renderStates[backBufferRenderSlot].depthStencilView);
	}
	else
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		d3d11Data->deviceContext->PSSetShaderResources(4, 1, &nullSRV);
	}
}

PS_IDX GetShadowMapPixelShader()
{
	return shadowMapPixelShader;
}
