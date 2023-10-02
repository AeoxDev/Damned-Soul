#pragma once
#include <d3d11.h>
#include <cinttypes>
#include <MemLib\ML_Map.hpp>
// For Image
#include "STB_Helper.h"
//This is the back end


struct D3D11Data
{
	ID3D11Device*			device;
	ID3D11DeviceContext*	deviceContext;
	IDXGISwapChain*			swapChain;

};

struct GeometryShaderHolder
{
	uint8_t									_nextIdx = 0;
	ML_Map<uint8_t, ID3D11GeometryShader*>	gs_map;
};


struct PixelShaderHolder
{
	uint8_t								_nextIdx = 0;
	ML_Map<uint8_t, ID3D11PixelShader*> ps_map;
};

struct VertexShaderHolder
{
	uint8_t									_nextIdx = 0;
	ML_Map<uint8_t, ID3D11VertexShader*>	vs_map;
	ML_Map < uint8_t, ID3D11InputLayout*>	il_map;
};

struct ComputeShaderHolder
{
	uint8_t									_nextIdx = 0;
	ML_Map < uint8_t, ID3D11ComputeShader*>	cs_map;
};

struct TextureHolder
{
	uint16_t										_nextIdx = 0; // How many textures are currently loaded
	ML_Map < uint16_t, uint64_t>					hash_map; // Used to check if Texture has already been loaded
	ML_Map < uint16_t, Image>						img_map; // Unnessecary?
	ML_Map < uint16_t, ID3D11Texture2D*>			tx_map; // Used to store the Textures, Unnessecary?
	ML_Map < uint16_t, ID3D11ShaderResourceView*>	srv_map; // Ysed to store the Shader Resource Views for the textures
};

struct SamplerStateHolder
{
	uint8_t _nextIdx = 0;
	ML_Map < uint8_t, ID3D11SamplerState*> smp_map;
};

struct BufferHolder
{
	uint16_t								_nextIdx = 0;
	ML_Map < uint16_t, ID3D11Buffer*>	buff_map; // MANY more buffers are needed than shaders
	ML_Map < uint16_t, uint32_t>	size; // Size
};

struct ViewPortHolder
{
	uint8_t								_nextIdx = 0;
	ML_Map < uint8_t, D3D11_VIEWPORT>	vp_map; // We, most likely, will not require a lot of view ports
};

struct RTVHolder
{
	uint8_t										_nextIdx = 0;
	ML_Map < uint8_t, ID3D11Texture2D*	>		tx_map; // Needs a texture for the depth stencil as well
	ML_Map < uint8_t, ID3D11RenderTargetView*>	rtv_map; // Since we are not using deferred rendering, we probably wont use a lot of these, but we definitely will use more than one
};

struct DSVHolder
{
	uint8_t										_nextIdx = 0;
	ML_Map < uint8_t, ID3D11Texture2D*	>		ds_map; // Needs a texture for the depth stencil as well
	ML_Map < uint8_t, ID3D11DepthStencilView*>	dsv_map; // Since we are not using deferred rendering, we probably wont use a lot of these, but we definitely will use more than one
};

struct SRVHolder
{
	uint8_t											_nextIdx = 0;
	ML_Map < uint8_t, ID3D11Resource*>				srv_resource_map; // Sometimes SRVs are created with textures, other times buffers therefore a resource is used
	ML_Map < uint8_t, ID3D11ShaderResourceView*>	srv_map; // NOTE: I dont think 8 is enough, will probably need more in the future
	ML_Map < uint8_t, uint32_t>						size; // Size
};

struct UAVHolder
{
	uint8_t											_nextIdx = 0;
	ML_Map < uint8_t, ID3D11Resource*>				uav_resource_map; // Sometimes SRVs are created with textures, other times buffers therefore a resource is used
	ML_Map < uint8_t, ID3D11UnorderedAccessView*>	uav_map; // NOTE: I dont think 8 is enough, will probably need more in the future
	ML_Map < uint8_t, uint32_t>						size; // Size
};

struct RasterizerHolder
{
	uint8_t										_nextIdx = 0;
	ML_Map < uint8_t, ID3D11RasterizerState*>	rs_map;
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


enum TEXTURE_HOLDER_TYPE
{
	TEXTURE,
	SHADER_RESOURCE_VIEW,
	RENDER_TARGET_VIEW,
	UNORDERED_ACCESS_VIEW
};

void GetTextureByType(ID3D11Texture2D*& out, TEXTURE_HOLDER_TYPE type, int16_t idx);