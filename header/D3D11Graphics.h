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

struct ComputeShaderHolder
{
	uint8_t currentCount = 0;
	ID3D11ComputeShader* cs_arr[16];
};

struct BufferHolder
{
	uint8_t currentCount = 0;
	ID3D11Buffer* buff_arr[512]; // MANY more buffers are needed than shaders
	uint32_t metadata_arr[512][3]; // Buffers needs an indicator on where and how to be used
};

struct ViewPortHolder
{
	uint8_t currentCount = 0;
	D3D11_VIEWPORT vp_arr[4]; // We, most likely, will not require a lot of view ports
};

struct RTVHolder
{
	uint8_t currentCount = 0;
	ID3D11RenderTargetView* rtv_arr[8]; // Since we are not using deferred rendering, we probably wont use a lot of these, but we definitely will use more than one
};

struct DSVHolder
{
	uint8_t currentCount = 0;
	ID3D11Texture2D* ds_arr[8]; // Needs a texture for the depth stencil as well
	ID3D11DepthStencilView* dsv_arr[8]; // Since we are not using deferred rendering, we probably wont use a lot of these, but we definitely will use more than one
};

struct SRVHolder
{
	uint8_t currentCount = 0;
	ID3D11Resource* srv_resource_arr[8]; // Sometimes SRVs are created with textures, other times buffers therefore a resource is used
	ID3D11ShaderResourceView* srv_arr[8]; // NOTE: I dont think 8 is enough, will probably need more in the future
	uint32_t metadata_arr[8][3]; // Shader Resource View needs an indicator on where and how to be used
};

struct UAVHolder
{
	uint8_t currentCount = 0;
	ID3D11Resource* uav_resource_arr[8]; // Sometimes SRVs are created with textures, other times buffers therefore a resource is used
	ID3D11UnorderedAccessView* uav_arr[8]; // NOTE: I dont think 8 is enough, will probably need more in the future
	uint32_t metadata_arr[8][2]; // Unordered Access View needs an indicator on how to be used, UAVs can only bind to compute shaders therefore no indication of where to use is needed
};

struct RasterizerHolder
{
	uint8_t currentCount = 0;
	ID3D11RasterizerState* rs_arr[4];
};

// Uses MemLib's persistent stack
extern D3D11Data* d3d11Data;
extern PixelShaderHolder* pixHolder;
extern VertexShaderHolder* vrtHolder;
extern ComputeShaderHolder* comHolder;
extern BufferHolder* bfrHolder;
extern ViewPortHolder* vpHolder;
extern RTVHolder* rtvHolder;
extern DSVHolder* dsvHolder;
extern SRVHolder* srvHolder;
extern UAVHolder* uavHolder;
extern RasterizerHolder* rsHolder;

//extern ID3D11Device* device;
//extern ID3D11DeviceContext* deviceContext;
//extern IDXGISwapChain* swapChain;

/// <summary>/// /// </summary>/// <param name="window"> use sdl.window</param>/// <returns></returns>
int SetupDirectX(HWND& window);

// Frees DirectX related memory
void EndDirectX();