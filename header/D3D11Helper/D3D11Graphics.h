#pragma once
#include <d3d11.h>
#include <cinttypes>
#include <MemLib\ML_Map.hpp>

// For _IDX types
#include "IDX_Types.h"
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
private:
	GS_IDX									_nextIdx = 0;
public:
	ML_Map<GS_IDX, ID3D11GeometryShader*>	gs_map;

	GS_IDX NextIdx();
};


struct PixelShaderHolder
{
private:
	PS_IDX								_nextIdx = 0;
public:
	ML_Map<PS_IDX, ID3D11PixelShader*>	ps_map;

	PS_IDX NextIdx();
};

struct VertexShaderHolder
{
private:
	VS_IDX								_nextIdx = 0;
public:
	ML_Map<VS_IDX, ID3D11VertexShader*>	vs_map;
	ML_Map<VS_IDX, ID3D11InputLayout*>	il_map;

	VS_IDX NextIdx();
};

struct ComputeShaderHolder
{
private:
	CS_IDX									_nextIdx = 0;
public:
	ML_Map <CS_IDX, ID3D11ComputeShader*>	cs_map;

	CS_IDX NextIdx();
};

struct TextureHolder
{
private:
	TX_IDX										_nextIdx = 0; // How many textures are currently loaded
public:
	ML_Map<TX_IDX, uint64_t>					hash_map; // Used to check if Texture has already been loaded
	//ML_Map<TX_IDX, Image>						img_map; // Unnessecary?
	ML_Map<TX_IDX, ID3D11Texture2D*>			tx_map; // Used to store the Textures, Unnessecary?
	ML_Map<TX_IDX, ID3D11ShaderResourceView*>	srv_map; // Ysed to store the Shader Resource Views for the textures

	TX_IDX NextIdx();
};

struct SamplerStateHolder
{
private:
	SMP_IDX _nextIdx = 0;
public:
	ML_Map<SMP_IDX, ID3D11SamplerState*> smp_map;

	SMP_IDX NextIdx();
};

struct BufferHolder
{
private:
	uint16_t							_nextIdx = 0;
public:
	ML_Map < uint16_t, ID3D11Buffer*>	buff_map; // MANY more buffers are needed than shaders
	ML_Map < uint16_t, uint32_t>		size; // Size
	ML_Map < uint16_t, uint32_t>		count; // Count, used for structured buffers

	uint16_t NextIdx();
};

struct ViewPortHolder
{
private:
	VP_IDX							_nextIdx = 0;
public:
	ML_Map<VP_IDX, D3D11_VIEWPORT>	vp_map; // We, most likely, will not require a lot of view ports

	VP_IDX NextIdx();
};

struct RTVHolder
{
private:
	RTV_IDX										_nextIdx = 0;
public:
	ML_Map<RTV_IDX, ID3D11Texture2D*	>		tx_map; // Needs a texture for the depth stencil as well
	ML_Map<RTV_IDX, ID3D11RenderTargetView*>	rtv_map; // Since we are not using deferred rendering, we probably wont use a lot of these, but we definitely will use more than one

	RTV_IDX NextIdx();
};

struct DSVHolder
{
private:
	DSV_IDX										_nextIdx = 0;
public:
	ML_Map<DSV_IDX, ID3D11Texture2D*	>		ds_map; // Needs a texture for the depth stencil as well
	ML_Map<DSV_IDX, ID3D11DepthStencilView*>	dsv_map; // Since we are not using deferred rendering, we probably wont use a lot of these, but we definitely will use more than one

	DSV_IDX NextIdx();
};

struct SRVHolder
{
private:
	SRV_IDX											_nextIdx = 0;
public:
	ML_Map<SRV_IDX, ID3D11Resource*>				srv_resource_map; // Sometimes SRVs are created with textures, other times buffers therefore a resource is used
	ML_Map<SRV_IDX, ID3D11ShaderResourceView*>		srv_map; // NOTE: I dont think 8 is enough, will probably need more in the future
	ML_Map<SRV_IDX, uint32_t>						size; // Size

	SRV_IDX NextIdx();
};

struct UAVHolder
{
private:
	UAV_IDX											_nextIdx = 0;
public:
	ML_Map<UAV_IDX, ID3D11Resource*>				uav_resource_map; // Sometimes SRVs are created with textures, other times buffers therefore a resource is used
	ML_Map<UAV_IDX, ID3D11UnorderedAccessView*>		uav_map; // NOTE: I dont think 8 is enough, will probably need more in the future
	ML_Map<UAV_IDX, uint32_t>						size; // Size

	UAV_IDX NextIdx();
};

struct RasterizerHolder
{
private:
	RS_IDX									_nextIdx = 0;
public:
	ML_Map<RS_IDX, ID3D11RasterizerState*>	rs_map;

	RS_IDX NextIdx();
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