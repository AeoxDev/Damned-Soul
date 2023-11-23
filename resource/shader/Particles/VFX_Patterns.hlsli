#include "VFX_Header.hlsli"

sampler diffuseSampler_in : register(s3);
Texture2D backbufferTexture_in : register(t0);

Texture2D colorRampTexture_in : register(t1);
Texture2D diffuseTexture_in : register(t2);
Texture2D vornoiTexture_in : register(t3);
Texture2D noiseTexture_in : register(t4);
Texture2D shapeTexture_in : register(t5);
Texture2D maskTexture_in : register(t6);

cbuffer pannerInput : register(b0)
{
    float2 offsetXY_in; // Offset of the uv coordinates in x ( u ) and y ( v ).
    float2 screenResolution_in; // How big is the screen resolution?
    float panSpeed_in; // How fast do you want the panning to be. A multiplier.
};

// ######## PREDEFINED VFX BEHAVIORS, TINY TWEAKABILITY #########
float4 VFXFire(
in float4 backBuffer,
in float time,
in float2 uv,
in float fireMultiplier = 8.0f,
in float3 fireColor = float3(1.0f, 0.45f, 0.0f),
in float distortionVornoi = 0.0132f,
in float distortionNoise = 0.15f
)
{
    float4 vornoiTexture    = vornoiTexture_in.Sample(diffuseSampler_in, UVPan(offsetXY_in, panSpeed_in * 1.f, time, uv * 0.5f));
    float4 gNoiseTexture    = noiseTexture_in.Sample(diffuseSampler_in, UVPan(offsetXY_in, panSpeed_in * 0.8f, time, uv));
    float4 vornoiDiffuse    = vornoiTexture_in.Sample(diffuseSampler_in, UVPan(offsetXY_in, panSpeed_in, time, uv));
    
    
    float2 distortedUV = distortUV(distortionNoise, distortUV(distortionVornoi, uv, pow(vornoiTexture, 1.2f)), gNoiseTexture);
    float4 fireDistort = shapeTexture_in.Sample(diffuseSampler_in, distortedUV);
    
    return AlphaBlend(backBuffer, fireDistort.r, pow(vornoiDiffuse.rgb, 1.0f), fireColor * fireMultiplier);
}