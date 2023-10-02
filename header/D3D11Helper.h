#pragma once
#include <cinttypes>
#include "Format.h"
//This contains helper functions for setting up renderable objects
// and manipulating their render states.



enum SHADER_TO_BIND_RESOURCE
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

enum CPU_FLAGS
{
	NONE = 0,
	WRITE = 0x10000L,
	READ = 0x20000L
};

enum USAGE_FLAGS
{
	USAGE_DEFAULT = 0,
	USAGE_IMMUTABLE = 1,
	USAGE_DYNAMIC = 2,
	USAGE_STAGING = 3
};

enum RESOURCE_FLAGS
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

enum LAYOUT_DESC
{
	DEFAULT,
	SKELETAL,
	SCREEN,
	PARTICLE
};

enum TOPOLOGY
{
	TRIANGLELIST,
	POINTLIST
};

typedef int16_t TX_IDX;
typedef int8_t	PS_IDX;
typedef int8_t	VS_IDX;
typedef int8_t	GS_IDX;
typedef int8_t	CS_IDX;
typedef int16_t CB_IDX;
typedef int16_t VB_IDX;
typedef int16_t IB_IDX;
typedef int8_t	VP_IDX;
typedef int8_t RTV_IDX;
typedef int8_t DSV_IDX;
typedef int8_t SRV_IDX;
typedef int8_t UAV_IDX;
typedef int8_t	RS_IDX;
typedef int8_t SMP_IDX;

// Load a texture from a .png file and return a global index that can be used to reference it
TX_IDX LoadTexture(const char* name);
// Create an empty texture with specific attributes
TX_IDX CreateTexture(FORMAT format, USAGE_FLAGS useFlags, RESOURCE_FLAGS bindFlags, CPU_FLAGS cpuAcess, const size_t& width, const size_t& height);

// Set a material
bool SetTexture(const TX_IDX idx, const SHADER_TO_BIND_RESOURCE& shader, uint8_t slot);
// Release for a single texture, make sure you dont accidently release a texture that is being used by an RTV, SRV or UAV
void ReleaseTX(const TX_IDX idx);


// Create a Sampler
SMP_IDX CreateSamplerState();
// Set a sampler (currently only binds to the pixel shader
void SetSamplerState(const SMP_IDX idx);
// Release for a single sampler
void ReleaseSMP(const SMP_IDX idx);

// Load a Pixel Shader by name (ps.cso) and return a global index that can be used to reference it
PS_IDX LoadPixelShader(const char* name);
// Set a new pixel shader by index
bool SetPixelShader(const PS_IDX idx);
// Release for a single pixel shader
void ReleasePS(const PS_IDX idx);

// Load a Vertex Shader by name (vs.cso) and return a global index that can be used to reference it
// Also creates an input layout to accompany it
VS_IDX LoadVertexShader(const char* name, LAYOUT_DESC layout = LAYOUT_DESC::SKELETAL);
// Set a new vertex (and accompanying input layout) shader by index
bool SetVertexShader(const VS_IDX idx);
// Release for a single vertex shader
void ReleaseVS(const VS_IDX idx);

// Load a Compute Shader by name (cs.cso) and return a global index that can be used to reference it
CS_IDX LoadComputeShader(const char* name);
// Set a new compute shader by index
bool SetComputeShader(const CS_IDX idx);
// Sets compute shader to null
bool ResetComputeShader();
// Release for a single compute shader
void ReleaseCS(const CS_IDX idx);

// Load a Geometry Shader by name (cs.cso) and return a global index that can be used to reference it
GS_IDX LoadGeometryShader(const char* name);
// Set a new geometry shader by index
bool SetGeometryShader(const GS_IDX idx);
// Sets geometry shader to null
bool ResetGeometryShader();
// Release for a single geometry shader
void ReleaseGS(const GS_IDX idx);



// Create a constant buffer with provided data and return a unique index to it
CB_IDX CreateConstantBuffer(const void* data, const size_t size);
// Overload for when there is no initial data
CB_IDX CreateConstantBuffer(const size_t size);
// Set an active constant buffer by index (shader and slot data contained in buffer)
bool SetConstantBuffer(const CB_IDX idx, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot);
// Sets constant buffer of parameter slot to NULL in the shader of parameter bindto
bool UnloadConstantBuffer(const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot);
// Update a constant buffer by index with given data
bool UpdateConstantBuffer(const CB_IDX, const void* data);
// Update the world matrix, there needs to be only one
void UpdateWorldMatrix(const void* data, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot);
//No rotations, only position
void SetWorldMatrix(float x, float y, float z, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot);
void SetWorldMatrix(float x, float y, float z, float rotationY, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot);
void SetWorldMatrix(float x, float y, float z, float dirX, float dirY, float dirZ, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot);
// Release for a single constant buffer
void ReleaseCS(const CS_IDX idx);


// Create a Vertex Buffer with provided data and return a unique index to it
VB_IDX CreateVertexBuffer(const void* data, const size_t& size, const size_t& count, const USAGE_FLAGS& useFlags);
// Overload for when there is no initial data
VB_IDX CreateVertexBuffer(const size_t& size, const size_t& count, const USAGE_FLAGS& useFlags);
// Set an active Vertex Buffer buffer by index
bool SetVertexBuffer(const VB_IDX idx);
// Sets vertex buffer to NULL
bool UnloadVertexBuffer();
// Release for a single vertex buffer
void ReleaseVB(const VB_IDX idx);

// Create an Index Buffer with provided data and return a unique index to it
// Needs to take unsigned 32 bit integers as data, as that is the underlying DXGI format used
IB_IDX CreateIndexBuffer(const uint32_t* data, const size_t& size, const size_t& count);
// Set an active Index Buffer buffer by index
bool SetIndexBuffer(const IB_IDX idx);
// Sets index buffer to NULL
bool UnloadIndexBuffer();
// Release for a single index buffer
void ReleaseIB(const IB_IDX idx);

// Create a Sampler
SMP_IDX CreateSamplerState();

// Create a viewport
VP_IDX CreateViewport(const size_t& width, const size_t& height);
// Set an active viewport
bool SetViewport(const VP_IDX idx);
// Release for a single constant buffer
void ReleaseVP(const VP_IDX idx);

// Creates the backbuffer from swapchain
RTV_IDX CreateBackBuffer();
// Create a render target view
RTV_IDX CreateRenderTargetView(USAGE_FLAGS useFlags, RESOURCE_FLAGS bindFlags, CPU_FLAGS cpuAcess, const size_t& width, const size_t& height, FORMAT format = FORMAT::FORMAT_R8G8B8A8_UNORM);
// Release for a single render target view
void ReleaseRTV(const RTV_IDX idx);

// Create a depth stencil view
DSV_IDX CreateDepthStencil(const size_t& width, const size_t& height);
// Set a render target view and depth stencil view
bool SetRenderTargetViewAndDepthStencil(const RTV_IDX idx_rtv, const DSV_IDX idx_ds);
// Release for a single depth stencil view
void ReleaseDSV(const DSV_IDX idx);


// Create a shader resource view that holds a buffer
SRV_IDX CreateShaderResourceViewBuffer(const void* data, const size_t& size, const int amount, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags);
// Create a shader resource view that holds a texture
// Currently only supports Texture2D, support for other formats will be implemented if needed
SRV_IDX CreateShaderResourceViewTexture(const RESOURCES& resource, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const size_t& width, const size_t& height);
// Overload to create a Shader Resource View with an already existing texture from another resource
SRV_IDX CreateShaderResourceViewTexture(const int16_t sourceIdx, RESOURCE_FLAGS sourceResource);
// Set an active shader resource view buffer by index (shader and slot data contained in buffer)
bool SetShaderResourceView(const SRV_IDX idx, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot);
// Sets shader resource view of parameter slot to NULL in the shader of parameter bindto
bool UnloadShaderResourceView(const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot);
// Release for a shader resource view
void ReleaseSRV(const SRV_IDX idx);

// Copies the data in a SRV to a vertex buffer, for particles
void CopyToVertexBuffer(const CB_IDX destination, const SRV_IDX source);

// Create a shader resource view that holds a buffer
SRV_IDX CreateUnorderedAccessViewBuffer(const void* data, const size_t& size, const int amount, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags);
// Overload to create a Unordered Access View with an already existing buffer from a Shader Resource View
SRV_IDX CreateUnorderedAccessViewBuffer(const size_t& size, const int amount, const int16_t idx);
// Create a shader resource view that holds a texture 
// NOT DEFINED, WILL BE DEFINED IF NEEDED
SRV_IDX CreateUnorderedAccessViewTexture(const RESOURCES& resource, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const size_t& width, const size_t& height);
// Set an active unordered access view buffer by index (shader and slot data contained in buffer)
bool SetUnorderedAcessView(const UAV_IDX idx, uint8_t slot);
// Sets unordered access view of parameter slot to NULL in the shader of parameter bindto
bool UnloadUnorderedAcessView(uint8_t slot);
// Release for a single unordered access view
void ReleaseUAV(const UAV_IDX idx);


// Clear the render target
void ClearRenderTargetView(const RTV_IDX idx, float r = 1.0f, float g = 0.f, float b = 0.84f, float a = 0.0f);
// Clear the depth stencil
void ClearDepthStencilView(const DSV_IDX idx);

RS_IDX CreateRasterizerState(const bool cull, const bool solid);
bool SetRasterizerState(const RS_IDX idx);
// Release for a single rasterizer state
void ReleaseRS(const RS_IDX idx);

void SetTopology(TOPOLOGY topology);

