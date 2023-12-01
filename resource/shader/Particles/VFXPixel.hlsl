#include "Headers/VFX_Patterns.hlsli" // NOTE: Constant Buffer, sampler and data resides in header.

struct PS_IN
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV;
    float time : TIME;
    int patterns : PATTERNS;
    int VFXpatterns : VFXPATTERNS;
};

float4 main(PS_IN inval) : SV_Target
{
    // NOTE: REQUIRED for Alpha Blending
    float4 backBuffer = SampleBackbuffer(inval.position, screenResolution_in, backbufferTexture_in, WrapSampler);

    return VFXAcidSpit(backBuffer, inval.time, inval.uv); // Default Red Fire //VFXSwordSlash(backBuffer, gs_in.time, gs_in.uv);
}

