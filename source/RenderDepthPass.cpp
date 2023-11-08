#include "RenderDepthPass.h"
#include "D3D11Helper\D3D11Helper.h"
#include "D3D11Helper\D3D11Graphics.h"
#include "SDLHandler.h"
#include "GameRenderer.h"

PS_IDX depthPassShader;

ID3D11Texture2D* depthPassTexture;
ID3D11RenderTargetView* depthPassRTV;
ID3D11ShaderResourceView* depthPassSRV;

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
	RESOURCE_FLAGS flags = (RESOURCE_FLAGS)((int)BIND_RENDER_TARGET | (int)BIND_SHADER_RESOURCE);
	uint8_t currentIdx = rtvHolder->NextIdx();

	D3D11_TEXTURE2D_DESC desc = { 0 };
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	// Take the height and width of the loaded image and set it as the dimensions for the texture
	desc.Width = (UINT)sdl.BASE_WIDTH;
	desc.Height = (UINT)sdl.BASE_HEIGHT;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = (D3D11_USAGE)D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.BindFlags = BIND_RENDER_TARGET | BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	// Attempt to create a texture in the device
	HRESULT hr = d3d11Data->device->CreateTexture2D(&desc, NULL, &depthPassTexture);
	assert(!FAILED(hr));

	hr = d3d11Data->device->CreateRenderTargetView(depthPassTexture, NULL, &depthPassRTV);
	assert(!FAILED(hr));

	hr = d3d11Data->device->CreateShaderResourceView(depthPassTexture, NULL, &depthPassSRV);
}

void SetDepthPassTexture(bool forRendering)
{
	if (forRendering)
	{
		float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		d3d11Data->deviceContext->ClearRenderTargetView(depthPassRTV, color);
		d3d11Data->deviceContext->OMSetRenderTargets(1, &depthPassRTV, dsvHolder->dsv_map[renderStates[backBufferRenderSlot].depthStencilView]);
		
	}
	else
	{
		d3d11Data->deviceContext->PSSetShaderResources(3, 1, &depthPassSRV);
	}
	
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
	depthPassRTV->Release();
	depthPassSRV->Release();
}
