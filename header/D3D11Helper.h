#pragma once
#include <cinttypes>
//This contains helper functions for setting up renderable objects
// and manipulating their render states.


enum SHADER_TO_BIND_BUFFER
{
	BIND_VERTEX,
	BIND_HULL,
	BIND_DOMAIN,
	BIND_GEOMETRY,
	BIND_PIXEL,
	BIND_COMPUTE
};

enum RESOURCES
{
	RESOURCE_BUFFER = 1,
	RESOURCE_TEXTURE1D = 2,
	RESOURCE_TEXTURE1DARRAY = 4,
	RESOURCE_TEXTURE2D = 8,
	RESOURCE_TEXTURE2DARRAY = 16,
	RESOURCE_TEXTURE3D = 32,
	RESOURCE_TEXCUBE = 64
};

enum FLAGS
{
	READ = 0x10000L,
	WRITE = 0x20000L
};

enum BUFFER_FLAGS
{
	BIND_VERTEX_BUFFER = 0x1L,
	BIND_INDEX_BUFFER = 0x2L,
	BIND_CONSTANT_BUFFER = 0x4L,
	BIND_SHADER_RESOURCE = 0x8L,
	BIND_STREAM_OUTPUT = 0x10L,
	BIND_RENDER_TARGET = 0x20L,
	BIND_DEPTH_STENCIL = 0x40L,
	BIND_UNORDERED_ACCESS = 0x80L,
	BIND_DECODER = 0x200L,
	BIND_VIDEO_ENCODER = 0x400L
};

typedef int8_t PS_IDX;
typedef int8_t VS_IDX;
typedef int8_t CS_IDX;
typedef int16_t CB_IDX;
typedef int16_t VB_IDX;
typedef int16_t IB_IDX;
typedef int8_t VP_IDX;
typedef int8_t RTV_IDX;
typedef int8_t DSV_IDX;
typedef int8_t SRV_IDX;
typedef int8_t UAV_IDX;
typedef int8_t RS_IDX;

// Load a Pixel Shader by name (ps.cso) and return a global index that can be used to reference it
PS_IDX LoadPixelShader(const char* name);
// Set a new pixel shader by index
bool SetPixelShader(const PS_IDX idx);

// Load a Vertex Shader by name (vs.cso) and return a global index that can be used to reference it
// Also creates an input layout to accompany it
VS_IDX LoadVertexShader(const char* name);
// Set a new vertex (and accompanying input layout) shader by index
bool SetVertexShader(const VS_IDX idx);

// Load a Compute Shader by name (cs.cso) and return a global index that can be used to reference it
CS_IDX LoadComputeShader(const char* name);
// Set a new compute shader by index
bool SetComputeShader(const CS_IDX idx);


// Create a constant buffer with provided data and return a unique index to it
CB_IDX CreateConstantBuffer(const void* data, const size_t size, const SHADER_TO_BIND_BUFFER& bindto, const uint8_t slot);
// Set an active constant buffer by index (shader and slot data contained in buffer)
bool SetConstantBuffer(const CB_IDX idx);
// Update a constant buffer by index with given data
bool UpdateConstantBuffer(const CB_IDX, const void* data);


// Create a Vertex Buffer with provided data and return a unique index to it
VB_IDX CreateVertexBuffer(const void* data, const size_t& size, const size_t& count);
// Set an active Vertex Buffer buffer by index
bool SetVertexBuffer(const VB_IDX idx);

// Create an Index Buffer with provided data and return a unique index to it
// Needs to take unsigned 32 bit integers as data, as that is the underlying DXGI format used
IB_IDX CreateIndexBuffer(const uint32_t* data, const size_t& size, const size_t& count);
// Set an active Index Buffer buffer by index
bool SetIndexBuffer(const IB_IDX idx);


// Create a viewport
VP_IDX CreateViewport(const size_t& width, const size_t& height);
// Set an active viewport
bool SetViewport(const VP_IDX idx);

// Create a render target view
RTV_IDX CreateRenderTargetView();
// Create a depth stencil view
DSV_IDX CreateDepthStencil(const size_t& width, const size_t& height);
// Set a render target view and depth stencil view
bool SetRenderTargetViewAndDepthStencil(const RTV_IDX idx_rtv, const DSV_IDX idx_ds);



//NOTE TODO: MAKE A FIX FOR CREATESRV AND CREATEUAV (THEY CAN NOT HAVE MULTIPLE BINDFLAGS THEY ARE HARDCODED)
// Create a shader resource view, if a buffer is to be created send in an empty string
SRV_IDX CreateShaderResourceView(const void* data, const size_t size, const SHADER_TO_BIND_BUFFER& bindto, const RESOURCES& resource, BUFFER_FLAGS bufferFlags, const FLAGS& flags, const uint8_t slot);
// Set an active shader resource view buffer by index (shader and slot data contained in buffer)
bool SetShaderResourceView(const SRV_IDX idx);
// Update a shader resource view index with given data
bool UpdateShaderResourceView(const SRV_IDX, const void* data);

// Create a unordered access view, if a buffer is to be created send in an empty string
UAV_IDX CreateUnorderedAcessView(const void* data, const size_t size, const RESOURCES &resource, BUFFER_FLAGS bufferFlags, const FLAGS &flags, const uint8_t slot);
// Set an active unordered access view buffer by index (shader and slot data contained in buffer)
bool SetUnorderedAcessView(const UAV_IDX idx);
// Update a unordered access view index with given data
//bool UpdateUnorderedAcessView(const UAV_IDX, const void* data);


// Clear the render target
void ClearRenderTargetView(const RTV_IDX idx);
// Clear the depth stencil
void ClearDepthStencilView(const DSV_IDX idx);

RS_IDX CreateRasterizerState(const bool cull, const bool solid);
bool SetRasterizerState(const RS_IDX idx);
