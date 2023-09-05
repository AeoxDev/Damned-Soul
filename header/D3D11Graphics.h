#pragma once
#include <d3d11.h>
#include <cinttypes>
//This is the back end

struct D3D11Data
{
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

};

struct PixelShaderHolder
{
	uint8_t currentCount = 0;
	ID3D11PixelShader* ps_arr[16];
};

struct VertexShaderHolder
{
	uint8_t currentCount = 0;
	ID3D11VertexShader* vs_arr[16];
	ID3D11InputLayout* il_arr[16];
};

// Uses MemLib's persistent stack
extern D3D11Data* d3d11Data;
extern PixelShaderHolder* pixHolder;
extern VertexShaderHolder* vrtHolder;

//extern ID3D11Device* device;
//extern ID3D11DeviceContext* deviceContext;
//extern IDXGISwapChain* swapChain;

/// <summary>/// /// </summary>/// <param name="window"> use sdl.window</param>/// <returns></returns>
int SetupDirectX(HWND& window);

void EndDirectX();