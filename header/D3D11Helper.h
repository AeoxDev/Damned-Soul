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

enum FORMAT;

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
VS_IDX LoadVertexShader(const char* name, LAYOUT_DESC layout = LAYOUT_DESC::SKELETAL);
// Set a new vertex (and accompanying input layout) shader by index
bool SetVertexShader(const VS_IDX idx);

// Load a Compute Shader by name (cs.cso) and return a global index that can be used to reference it
CS_IDX LoadComputeShader(const char* name);
// Set a new compute shader by index
bool SetComputeShader(const CS_IDX idx);
// Sets compute shader to null
bool ResetComputeShader();

// Load a Geometry Shader by name (cs.cso) and return a global index that can be used to reference it
GS_IDX LoadGeometryShader(const char* name);
// Set a new geometry shader by index
bool SetGeometryShader(const GS_IDX idx);
// Sets geometry shader to null
bool ResetGeometryShader();



// Create a constant buffer with provided data and return a unique index to it
CB_IDX CreateConstantBuffer(const void* data, const size_t size, const uint8_t slot);
// Set an active constant buffer by index (shader and slot data contained in buffer)
bool SetConstantBuffer(const CB_IDX idx, const SHADER_TO_BIND_RESOURCE& bindto);
//Overload that sets slot to NULL before setting buffer, for particles
bool SetConstantBuffer(const CB_IDX idx, const SHADER_TO_BIND_RESOURCE& bindto, bool particles);
// Update a constant buffer by index with given data
bool UpdateConstantBuffer(const CB_IDX, const void* data);
// Update the world matrix, there needs to be only one
void UpdateWorldMatrix(const void* data, const SHADER_TO_BIND_RESOURCE& bindto);


// Create a Vertex Buffer with provided data and return a unique index to it
VB_IDX CreateVertexBuffer(const void* data, const size_t& size, const size_t& count, const USAGE_FLAGS& useFlag);
// Set an active Vertex Buffer buffer by index
bool SetVertexBuffer(const VB_IDX idx);

// Create an Index Buffer with provided data and return a unique index to it
// Needs to take unsigned 32 bit integers as data, as that is the underlying DXGI format used
IB_IDX CreateIndexBuffer(const uint32_t* data, const size_t& size, const size_t& count);
// Set an active Index Buffer buffer by index
bool SetIndexBuffer(const IB_IDX idx);

// Create a Sampler
SMP_IDX CreateSamplerState();

// Create a viewport
VP_IDX CreateViewport(const size_t& width, const size_t& height);
// Set an active viewport
bool SetViewport(const VP_IDX idx);

// Creates the backbuffer from swapchain
RTV_IDX CreateBackBuffer();
// Create a render target view
RTV_IDX CreateRenderTargetView(USAGE_FLAGS useFlags, RESOURCE_FLAGS bindFlags, CPU_FLAGS cpuAcess, const size_t& width, const size_t& height);

// Create a depth stencil view
DSV_IDX CreateDepthStencil(const size_t& width, const size_t& height);
// Set a render target view and depth stencil view
bool SetRenderTargetViewAndDepthStencil(const RTV_IDX idx_rtv, const DSV_IDX idx_ds);


// Create a shader resource view that holds a buffer
SRV_IDX CreateShaderResourceViewBuffer(const void* data, const size_t& size, const int amount, const SHADER_TO_BIND_RESOURCE& bindto, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const uint8_t slot);
// Create a shader resource view that holds a texture
// Currently only supports Texture2D, support for other formats will be implemented if needed
SRV_IDX CreateShaderResourceViewTexture(const SHADER_TO_BIND_RESOURCE& bindto, const RESOURCES& resource, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const size_t& width, const size_t& height, const uint8_t slot);
// Overload to create a Shader Resource View with an already existing texture from another resource
SRV_IDX CreateShaderResourceViewTexture(const int16_t idx, const SHADER_TO_BIND_RESOURCE& bindto, RESOURCE_FLAGS sourceResource, const uint8_t slot);
// Set an active shader resource view buffer by index (shader and slot data contained in buffer)
bool SetShaderResourceView(const SRV_IDX idx);
//Overload that sets slot to NULL before setting SRV, for particles
bool UnloadShaderResourceView(const SRV_IDX idx);

// Copies the data in a SRV to a vertex buffer, for particles
void CopyToVertexBuffer(const CB_IDX destination, const SRV_IDX source);

// Create a shader resource view that holds a buffer
SRV_IDX CreateUnorderedAccessViewBuffer(const void* data, const size_t& size, const int amount, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const uint8_t slot);
// Overload to create a Unordered Access View with an already existing buffer from a Shader Resource View
SRV_IDX CreateUnorderedAccessViewBuffer(const size_t& size, const int amount, const int16_t idx, const uint8_t slot);
// Create a shader resource view that holds a texture 
// NOT DEFINED, WILL BE DEFINED IF NEEDED
SRV_IDX CreateUnorderedAccessViewTexture(const SHADER_TO_BIND_RESOURCE& bindto, const RESOURCES& resource, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const size_t& width, const size_t& height, const uint8_t slot);
// Set an active unordered access view buffer by index (shader and slot data contained in buffer)
bool SetUnorderedAcessView(const UAV_IDX idx);
//Overload that sets slot to NULL before setting UAV, for particles
bool UnloadUnorderedAcessView(const UAV_IDX idx);


// Clear the render target
void ClearRenderTargetView(const RTV_IDX idx);
// Clear the depth stencil
void ClearDepthStencilView(const DSV_IDX idx);

RS_IDX CreateRasterizerState(const bool cull, const bool solid);
bool SetRasterizerState(const RS_IDX idx);

void SetTopology(TOPOLOGY topology);

enum FORMAT
{
    FORMAT_UNKNOWN = 0,
    FORMAT_R32G32B32A32_TYPELESS = 1,
    FORMAT_R32G32B32A32_FLOAT = 2,
    FORMAT_R32G32B32A32_UINT = 3,
    FORMAT_R32G32B32A32_SINT = 4,
    FORMAT_R32G32B32_TYPELESS = 5,
    FORMAT_R32G32B32_FLOAT = 6,
    FORMAT_R32G32B32_UINT = 7,
    FORMAT_R32G32B32_SINT = 8,
    FORMAT_R16G16B16A16_TYPELESS = 9,
    FORMAT_R16G16B16A16_FLOAT = 10,
    FORMAT_R16G16B16A16_UNORM = 11,
    FORMAT_R16G16B16A16_UINT = 12,
    FORMAT_R16G16B16A16_SNORM = 13,
    FORMAT_R16G16B16A16_SINT = 14,
    FORMAT_R32G32_TYPELESS = 15,
    FORMAT_R32G32_FLOAT = 16,
    FORMAT_R32G32_UINT = 17,
    FORMAT_R32G32_SINT = 18,
    FORMAT_R32G8X24_TYPELESS = 19,
    FORMAT_D32_FLOAT_S8X24_UINT = 20,
    FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
    FORMAT_X32_TYPELESS_G8X24_UINT = 22,
    FORMAT_R10G10B10A2_TYPELESS = 23,
    FORMAT_R10G10B10A2_UNORM = 24,
    FORMAT_R10G10B10A2_UINT = 25,
    FORMAT_R11G11B10_FLOAT = 26,
    FORMAT_R8G8B8A8_TYPELESS = 27,
    FORMAT_R8G8B8A8_UNORM = 28,
    FORMAT_R8G8B8A8_UNORM_SRGB = 29,
    FORMAT_R8G8B8A8_UINT = 30,
    FORMAT_R8G8B8A8_SNORM = 31,
    FORMAT_R8G8B8A8_SINT = 32,
    FORMAT_R16G16_TYPELESS = 33,
    FORMAT_R16G16_FLOAT = 34,
    FORMAT_R16G16_UNORM = 35,
    FORMAT_R16G16_UINT = 36,
    FORMAT_R16G16_SNORM = 37,
    FORMAT_R16G16_SINT = 38,
    FORMAT_R32_TYPELESS = 39,
    FORMAT_D32_FLOAT = 40,
    FORMAT_R32_FLOAT = 41,
    FORMAT_R32_UINT = 42,
    FORMAT_R32_SINT = 43,
    FORMAT_R24G8_TYPELESS = 44,
    FORMAT_D24_UNORM_S8_UINT = 45,
    FORMAT_R24_UNORM_X8_TYPELESS = 46,
    FORMAT_X24_TYPELESS_G8_UINT = 47,
    FORMAT_R8G8_TYPELESS = 48,
    FORMAT_R8G8_UNORM = 49,
    FORMAT_R8G8_UINT = 50,
    FORMAT_R8G8_SNORM = 51,
    FORMAT_R8G8_SINT = 52,
    FORMAT_R16_TYPELESS = 53,
    FORMAT_R16_FLOAT = 54,
    FORMAT_D16_UNORM = 55,
    FORMAT_R16_UNORM = 56,
    FORMAT_R16_UINT = 57,
    FORMAT_R16_SNORM = 58,
    FORMAT_R16_SINT = 59,
    FORMAT_R8_TYPELESS = 60,
    FORMAT_R8_UNORM = 61,
    FORMAT_R8_UINT = 62,
    FORMAT_R8_SNORM = 63,
    FORMAT_R8_SINT = 64,
    FORMAT_A8_UNORM = 65,
    FORMAT_R1_UNORM = 66,
    FORMAT_R9G9B9E5_SHAREDEXP = 67,
    FORMAT_R8G8_B8G8_UNORM = 68,
    FORMAT_G8R8_G8B8_UNORM = 69,
    FORMAT_BC1_TYPELESS = 70,
    FORMAT_BC1_UNORM = 71,
    FORMAT_BC1_UNORM_SRGB = 72,
    FORMAT_BC2_TYPELESS = 73,
    FORMAT_BC2_UNORM = 74,
    FORMAT_BC2_UNORM_SRGB = 75,
    FORMAT_BC3_TYPELESS = 76,
    FORMAT_BC3_UNORM = 77,
    FORMAT_BC3_UNORM_SRGB = 78,
    FORMAT_BC4_TYPELESS = 79,
    FORMAT_BC4_UNORM = 80,
    FORMAT_BC4_SNORM = 81,
    FORMAT_BC5_TYPELESS = 82,
    FORMAT_BC5_UNORM = 83,
    FORMAT_BC5_SNORM = 84,
    FORMAT_B5G6R5_UNORM = 85,
    FORMAT_B5G5R5A1_UNORM = 86,
    FORMAT_B8G8R8A8_UNORM = 87,
    FORMAT_B8G8R8X8_UNORM = 88,
    FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
    FORMAT_B8G8R8A8_TYPELESS = 90,
    FORMAT_B8G8R8A8_UNORM_SRGB = 91,
    FORMAT_B8G8R8X8_TYPELESS = 92,
    FORMAT_B8G8R8X8_UNORM_SRGB = 93,
    FORMAT_BC6H_TYPELESS = 94,
    FORMAT_BC6H_UF16 = 95,
    FORMAT_BC6H_SF16 = 96,
    FORMAT_BC7_TYPELESS = 97,
    FORMAT_BC7_UNORM = 98,
    FORMAT_BC7_UNORM_SRGB = 99,
    FORMAT_AYUV = 100,
    FORMAT_Y410 = 101,
    FORMAT_Y416 = 102,
    FORMAT_NV12 = 103,
    FORMAT_P010 = 104,
    FORMAT_P016 = 105,
    FORMAT_420_OPAQUE = 106,
    FORMAT_YUY2 = 107,
    FORMAT_Y210 = 108,
    FORMAT_Y216 = 109,
    FORMAT_NV11 = 110,
    FORMAT_AI44 = 111,
    FORMAT_IA44 = 112,
    FORMAT_P8 = 113,
    FORMAT_A8P8 = 114,
    FORMAT_B4G4R4A4_UNORM = 115,

    FORMAT_P208 = 130,
    FORMAT_V208 = 131,
    FORMAT_V408 = 132,


    FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE = 189,
    FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE = 190,


    FORMAT_FORCE_UINT = 0xffffffff
};