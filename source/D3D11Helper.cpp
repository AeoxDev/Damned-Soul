#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include <iostream>
#include <fstream>



int8_t LoadPixelShader(const char* name)//(ID3D11PixelShader* pixelShader)
{
	std::ifstream reader;

	reader.open(name, std::ios::binary | std::ios::ate);
	if (false == reader.is_open())
	{
		std::cerr << "Could not open PS test file!" << std::endl;
		return false;
	}

	// Allocate the byte data on the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* shaderData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(shaderData, size);
	reader.close();

	HRESULT hr = d3d11Data->device->CreatePixelShader(shaderData, size, NULL, &pixHolder->ps_arr[pixHolder->currentCount]);
	if (FAILED(hr))
	{
		MemLib::spop(); // Free if failiure
		std::cerr << "Failed to create PS test shader!" << std::endl;
		return -1;
	}

	MemLib::spop(); // Free if it succeeded because it is destined to die
	return pixHolder->currentCount++;
}

bool SetPixelShader(int8_t idx)
{
	if (pixHolder->currentCount < idx)
	{
		std::cerr << "Failed to set pixel shader: Index out of range!" << std::endl;
		return false;
	}
	else if (idx < 0)
	{
		std::cerr << "Failed to set pixel shader: Index must be 0 or greater!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->PSSetShader(pixHolder->ps_arr[idx], nullptr, 0);
	return true;
}

// Does this string reference create more memory used on the stack?
bool CreateInputLayout(const char* vShaderByteCode, const unsigned int& size, ID3D11InputLayout*& il, const bool skeletonLayout)
{
	D3D11_INPUT_ELEMENT_DESC nonSkeletalInputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC skeletalInputDesc[5] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONE_WIEGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0},

	};

	D3D11_INPUT_ELEMENT_DESC* chosen = skeletonLayout ? skeletalInputDesc : nonSkeletalInputDesc;

	HRESULT hr = d3d11Data->device->CreateInputLayout(skeletalInputDesc, 3, vShaderByteCode, size, &il);

	if (FAILED(hr))
	{
		std::cerr << "Failed to create Input Layout!" << std::endl;
		return false;
	}

	return true;
}

int8_t LoadVertexShader(const char* name)
{
	std::ifstream reader;

	reader.open(name, std::ios::binary | std::ios::ate);
	if (false == reader.is_open())
	{
		std::cerr << "Could not open VS test file!" << std::endl;
		return false;
	}

	// Allocate the byte data on the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* shaderData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(shaderData, size);
	reader.close();

	HRESULT hr = d3d11Data->device->CreateVertexShader(shaderData, size, NULL, &vrtHolder->vs_arr[vrtHolder->currentCount]); // Does not increment here
	if (FAILED(hr))
	{
		MemLib::spop(); // Pop if failiure
		// No need to decrement here since no increment has yet to happen
		std::cerr << "Failed to create Vertex Shader!" << std::endl;
		return -1;
	}

	// Try to create accompanying input layout
	if (false == CreateInputLayout(shaderData, size, vrtHolder->il_arr[vrtHolder->currentCount], false))
	{
		MemLib::spop(); // Pop if failiure
		// No need to decrement here since no increment has yet to happen
		std::cerr << "Failed to create Input Layout for Vertex Shader!" << std::endl;
		return -1;
	}

	// Free the temp memory
	MemLib::spop();

	// Return and increment (in that order)
	return vrtHolder->currentCount++;
}

bool SetVertexShader(int8_t idx)
{
	if (vrtHolder->currentCount < idx)
	{
		std::cerr << "Failed to set vertex shader/input layout pair: Index out of range!" << std::endl;
		return false;
	}
	else if (idx < 0)
	{
		std::cerr << "Failed to set vertex shader/input layout pair: Index must be 0 or greater!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->VSSetShader(vrtHolder->vs_arr[idx], nullptr, 0);
	d3d11Data->deviceContext->IASetInputLayout(vrtHolder->il_arr[idx]);
	return true;
}

void SetViewport(D3D11_VIEWPORT& viewport, UINT width, UINT height)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}


bool CreateRenderTargetView(ID3D11Texture2D*& backBuffer, ID3D11RenderTargetView*& rtv)
{
	// get the address of the back buffer
	/*ID3D11Texture2D* */backBuffer = nullptr;
	if (FAILED(d3d11Data->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}

	// use the back buffer address to create the render target
	// null as description to base it on the backbuffers values
	HRESULT hr = d3d11Data->device->CreateRenderTargetView(backBuffer, NULL, &rtv);
	backBuffer->Release();

	if (FAILED(hr))
	{
		std::cerr << "Failed to create Render Target View!" << std::endl;
		return false;
	}
	return true;

}


bool CreateDepthStencil(UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView)
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

	HRESULT hr = d3d11Data->device->CreateTexture2D(&textureDesc, nullptr, &dsTexture);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Depth Stencil Texture!" << std::endl;
		return false;
	}

	hr = d3d11Data->device->CreateDepthStencilView(dsTexture, 0, &dsView);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Depth Stencil View!" << std::endl;
		return false;
	}

	return true;
}

	//if (false == LoadPixelShader(tps))
	//	FAIL_MSG
	//deviceContext->PSSetShader(tps, nullptr, 0);

	//std::string byteCode;
	//if (false == LoadVertexShader(tvs, byteCode))
	//	FAIL_MSG
	//deviceContext->VSSetShader(tvs, nullptr, 0);

	//CreateInputLayout(til, byteCode);
	//deviceContext->IASetInputLayout(til);

	//D3D11_VIEWPORT vp;
	//SetViewport(vp, WIDTH, HEIGHT);
	//deviceContext->RSSetViewports(1, &vp);

	//if (false == CreateDepthStencil(WIDTH, HEIGHT, tdst, tdsv))
	//	FAIL_MSG

	//if (false == CreateRenderTargetView(tbb, trtv))
	//	FAIL_MSG
	//deviceContext->OMSetRenderTargets(1, &trtv, tdsv);

	//float color[4] = { 1.0f, 0.0f, 0.8235f, 1.0f };
	//deviceContext->ClearRenderTargetView(trtv, color);
	//deviceContext->Draw(0, 0);
	//swapChain->Present(0, 0);