#pragma once
#include <d3d11.h>
#include <cinttypes>
// For Image
#include "STB_Helper.h"
//This is the back end

struct D3D11Data
{
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

};

struct GeometryShaderHolder
{
	uint8_t currentCount = 0;
	ID3D11GeometryShader* gs_arr[16];
};

struct TextureHolder
{
#define TX_HOLD_LIM 64
	uint16_t					currentCount = 0; // How many textures are currently loaded
	uint64_t					hash_arr[TX_HOLD_LIM]; // Used to check if Texture has already been loaded
	Image						img_arr[TX_HOLD_LIM]; // Unnessecary?
	ID3D11Texture2D*			tx_arr[TX_HOLD_LIM]; // Used to store the Textures, Unnessecary?
	ID3D11ShaderResourceView*	srv_arr[TX_HOLD_LIM]; // Ysed to store the Shader Resource Views for the textures
};

struct SamplerStateHolder
{
	uint8_t currentCount = 0;
	ID3D11SamplerState* smp_arr[4];
};

struct PixelShaderHolder
{
#define PS_HOLD_LIM 16
	uint8_t				currentCount = 0;
	ID3D11PixelShader*	ps_arr[PS_HOLD_LIM];
};

struct VertexShaderHolder
{
#define VS_HOLD_LIM 16
	uint8_t					currentCount = 0;
	ID3D11VertexShader*		vs_arr[VS_HOLD_LIM];
	ID3D11InputLayout*		il_arr[VS_HOLD_LIM];
};

struct ComputeShaderHolder
{
#define CS_HOLD_LIM 16
	uint8_t					currentCount = 0;
	ID3D11ComputeShader*	cs_arr[CS_HOLD_LIM];
};

struct BufferHolder
{
#define BUFF_HOLD_LIM 512
	uint8_t			currentCount = 0;
	ID3D11Buffer*	buff_arr[BUFF_HOLD_LIM]; // MANY more buffers are needed than shaders
	uint32_t		metadata_arr[BUFF_HOLD_LIM][3]; // Buffers needs an indicator on where and how to be used
};

struct ViewPortHolder
{
#define VP_HOLD_LIM 4
	uint8_t			currentCount = 0;
	D3D11_VIEWPORT	vp_arr[VP_HOLD_LIM]; // We, most likely, will not require a lot of view ports
};

struct RTVHolder
{
#define RTV_HOLD_LIM 8
	uint8_t					currentCount = 0;
	ID3D11Texture2D*		tx_arr[RTV_HOLD_LIM]; // Needs a texture for the depth stencil as well
	ID3D11RenderTargetView*	rtv_arr[RTV_HOLD_LIM]; // Since we are not using deferred rendering, we probably wont use a lot of these, but we definitely will use more than one
};

struct DSVHolder
{
#define DSV_HOLD_LIM 8
	uint8_t					currentCount = 0;
	ID3D11Texture2D*		ds_arr[DSV_HOLD_LIM]; // Needs a texture for the depth stencil as well
	ID3D11DepthStencilView*	dsv_arr[DSV_HOLD_LIM]; // Since we are not using deferred rendering, we probably wont use a lot of these, but we definitely will use more than one
};

struct SRVHolder
{
#define SRV_HOLD_LIM 8
	uint8_t						currentCount = 0;
	ID3D11Resource*				srv_resource_arr[SRV_HOLD_LIM]; // Sometimes SRVs are created with textures, other times buffers therefore a resource is used
	ID3D11ShaderResourceView*	srv_arr[SRV_HOLD_LIM]; // NOTE: I dont think 8 is enough, will probably need more in the future
	uint32_t					metadata_arr[SRV_HOLD_LIM][3]; // Shader Resource View needs an indicator on where and how to be used
};

struct UAVHolder
{
#define UAV_HOLD_LIM 8
	uint8_t						currentCount = 0;
	ID3D11Resource*				uav_resource_arr[UAV_HOLD_LIM]; // Sometimes SRVs are created with textures, other times buffers therefore a resource is used
	ID3D11UnorderedAccessView*	uav_arr[UAV_HOLD_LIM]; // NOTE: I dont think 8 is enough, will probably need more in the future
	uint32_t					metadata_arr[UAV_HOLD_LIM][2]; // Unordered Access View needs an indicator on how to be used, UAVs can only bind to compute shaders therefore no indication of where to use is needed
};

struct RasterizerHolder
{
#define RS_HOLD_LIM 4
	uint8_t					currentCount = 0;
	ID3D11RasterizerState*	rs_arr[RS_HOLD_LIM];
};

// Uses MemLib's persistent stack
extern D3D11Data* d3d11Data;
extern TextureHolder* txHolder;
extern SamplerStateHolder* smpHolder;
extern PixelShaderHolder* pixHolder;
extern VertexShaderHolder* vrtHolder;
extern GeometryShaderHolder* geoHolder;
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