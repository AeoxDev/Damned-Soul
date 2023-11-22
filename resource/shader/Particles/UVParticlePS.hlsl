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
    float2 uvCoordinates = float2(-gs_in.uv.x - frac(offsetXY_in.x * gs_in.time * panSpeed_in), gs_in.uv.y - frac(offsetXY_in.y * gs_in.time * panSpeed_in));
    float4 vornoiTexture = vornoiTexture_in.Sample(diffuseSampler_in, UVPan(offsetXY_in, panSpeed_in, gs_in.time, gs_in.uv));
    
    float distortionAmount = 0.132f;
    float4 lerpingTexture = lerp(float4(gs_in.uv, 0.0f, 0.0f), vornoiTexture, distortionAmount);
    
    float4 diffuseTexture = diffuseTexture_in.Sample(diffuseSampler_in, gs_in.uv);
    
    float4 mask = maskTexture_in.Sample(diffuseSampler_in, lerpingTexture.xy);
    float2 newUV = float2((gs_in.position.x / screenResolution_in.x), ( gs_in.position.y / screenResolution_in.y));
    
    float3 results;
    results.r = backbufferTexture_in.Sample(diffuseSampler_in, newUV).r * ( 1 - mask.r) + (mask.r * 1.0f * mask.r);
    results.g = backbufferTexture_in.Sample(diffuseSampler_in, newUV).g * ( 1 - mask.r) + (mask.b * 0.2f * mask.r);
    results.b = backbufferTexture_in.Sample(diffuseSampler_in, newUV).b * ( 1 - mask.r) + (mask.g * 0.0f * mask.r);
    
    
    return float4(saturate(results.rgb), 1.0);
}

