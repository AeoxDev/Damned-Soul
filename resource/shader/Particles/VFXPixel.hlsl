#include "VFX_Patterns.hlsli" // NOTE: Constant Buffer, sampler and data resides in header.

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV_COORDINATES;
    float time : TIME;
};

float4 main(GS_OUT gs_in) : SV_Target
{
    // NOTE: REQUIRED for Alpha Blending
    float4 backBuffer = SampleBackbuffer(gs_in.position, screenResolution_in, backbufferTexture_in, vfxSampler_in);

    return VFXSwordSlash(backBuffer, gs_in.time, gs_in.uv); // Default Red Fire //VFXSwordSlash(backBuffer, gs_in.time, gs_in.uv);
}

