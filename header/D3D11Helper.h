#pragma once
#include <cinttypes>
//This contains helper functions for setting up renderable objects
// and manipulating their render states.

enum SHADER_TO_BIND_RESOURCE
{
	BIND_VERTEX,
	BIND_HULL,
	BIND_DOMAIN,
	BIND_GEOMETRY,
	BIND_PIXEL
};

typedef int16_t TX_IDX;
typedef int8_t SMP_IDX;
typedef int8_t PS_IDX;
typedef int8_t VS_IDX;
typedef int8_t CS_IDX;
typedef int16_t CB_IDX;
typedef int16_t VB_IDX;
typedef int16_t IB_IDX;
typedef int8_t VP_IDX;
typedef int8_t RTV_IDX;
typedef int8_t DSV_IDX;
typedef int8_t RS_IDX;

// Load a texture from a .png file and return a global index that can be used to reference it
TX_IDX LoadTexture(const char* name);
// Set a material
bool SetTexture(const TX_IDX idx, const uint8_t slot, const SHADER_TO_BIND_RESOURCE& shader);

// Create a Sampler
SMP_IDX CreateSamplerState();
// Set a sampler (currently only binds to the pixel shader
void SetSamplerState(const SMP_IDX idx);

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
CS_IDX LoadVertexShader(const char* name);
// Set a new compute shader by index
bool SetVertexShader(const VS_IDX idx);


// Create a constant buffer with provided data and return a unique index to it
CB_IDX CreateConstantBuffer(const void* data, const size_t size, const SHADER_TO_BIND_RESOURCE& bindto, const uint8_t slot);
// Set an active constant buffer by index (shader and slot data contained in buffer)
bool SetConstantBuffer(const CB_IDX idx);
// Update a constant buffer by index with given data
bool UpdateConstantBuffer(const CB_IDX, const void* data);
// Update the world matrix, there needs to be only one
void UpdateWorldMatrix(const void* data);

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

// Clear the render target
void ClearRenderTargetView(const RTV_IDX idx);
// Clear the depth stencil
void ClearDepthStencilView(const DSV_IDX idx);

RS_IDX CreateRasterizerState(const bool cull, const bool solid);
bool SetRasterizerState(const RS_IDX idx);
