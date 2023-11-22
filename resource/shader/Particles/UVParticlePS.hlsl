#include "UVParticleDistortions.hlsli"

sampler diffuseSampler_in : register(s3);
Texture2D backbufferTexture_in : register(t0);
Texture2D diffuseTexture_in : register(t1);
Texture2D vornoiTexture_in : register(t2);
Texture2D maskTexture_in : register(t3);

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV_COORDINATES;
    float time : TIME;
};

cbuffer pannerInput: register(b0) {
    float2 offsetXY_in; // Offset of the uv coordinates in x ( u ) and y ( v ).
    float2 screenResolution_in; // How big is the screen resolution?
    float panSpeed_in; // How fast do you want the panning to be. A multiplier.
};

float4 main(GS_OUT gs_in) : SV_Target
{
    float4 vornoiTexture = vornoiTexture_in.Sample(diffuseSampler_in, UVPan(offsetXY_in, panSpeed_in, gs_in.time, gs_in.uv));
    float4 mask = maskTexture_in.Sample(diffuseSampler_in, distortUV(0.132f, gs_in.uv, vornoiTexture));
    float4 backBuffer = SampleBackbuffer(gs_in.position, screenResolution_in, backbufferTexture_in, diffuseSampler_in);
    
    float3 results;
    results.r = backBuffer.r * ( 1 - mask.r) + (mask.r * 1.0f * mask.r);
    results.g = backBuffer.g * ( 1 - mask.r) + (mask.b * 0.2f * mask.r);
    results.b = backBuffer.b * ( 1 - mask.r) + (mask.g * 0.0f * mask.r);
    
    // TODO: Make the Blending be in a function instead of here.
    
    return float4(saturate(results.rgb), 1.0);
}

