#include "VFX_Header.hlsli"

sampler vfxSampler_in : register(s3);
Texture2D backbufferTexture_in : register(t0);

Texture2D colorRampTexture_in : register(t1);
Texture2D vornoiTexture_in : register(t2);
Texture2D noiseTexture_in : register(t3);
Texture2D shapeTexture_in : register(t4);
Texture2D maskTexture_in : register(t5);

cbuffer pannerInput : register(b0)
{
    float2 offsetXY_in; // Offset of the uv coordinates in x ( u ) and y ( v ).
    float2 screenResolution_in; // How big is the screen resolution?
    float panSpeed_in; // How fast do you want the panning to be. A multiplier.
};

// ######## PREDEFINED VFX BEHAVIORS, TINY TWEAKABILITY #########


// FIRE REQUIRES: Backbuffer, VFX_Vornoi, VFX_gNoise, VFX_FireGradient & VFX_SoftCircle
float4 VFXFire( 
in float4 backBuffer,
in float time,
in float2 uv,
in float fireMultiplier = 1.0f,
in float3 fireColor = float3(0.0f, 0.0f, 0.0f),
in float distortionVornoi = 0.132f,
in float distortionNoise = 0.1f
)
{
    float4 vornoiTexture = vornoiTexture_in.Sample(vfxSampler_in, UVPan(offsetXY_in, panSpeed_in * 1.f, time, uv * 0.5f));
    float4 gNoiseTexture = noiseTexture_in.Sample(vfxSampler_in, UVPan(offsetXY_in, panSpeed_in * 0.9f, time, uv));
    float4 vornoiDiffuse = vornoiTexture_in.Sample(vfxSampler_in, UVPan(offsetXY_in, panSpeed_in, time, uv));
    
    
    float2 distortedUV = distortUV(distortionNoise, distortUV(distortionVornoi, uv, pow(vornoiTexture, 1.2f)), gNoiseTexture);
    float4 fireDistort = shapeTexture_in.Sample(vfxSampler_in, distortedUV);
    float3 color = SampleColorRamp(colorRampTexture_in, vfxSampler_in, fireDistort);
    
    return AlphaBlend(backBuffer, fireDistort.r, color + (fireColor * fireMultiplier));
}

// ACID REQUIRES: Backbuffer, VFX_Vornoi, VFX_gNoise, VFX_FireGradient & VFX_SoftCircle
// NOTE: This function is more "hard coded" as it only "technically" is used by the acidspitting.
float4 VFXAcidSpit(
in float4 backBuffer,
in float time,
in float2 uv,
in float fireMultiplier = 4.0f,
in float3 fireColor = float3(0.2f, 1.0f, 0.2f),
in float distortionVornoi = 0.232f,
in float distortionNoise = 0.01f
)
{
    float4 vornoiTexture = vornoiTexture_in.Sample(vfxSampler_in,   UVPan(float2(0.0f, -1.0f), 1.25f,    time, uv * 0.5f));
    float4 gNoiseTexture = noiseTexture_in.Sample(vfxSampler_in,    UVPan(float2(0.0f, -1.0f),  1.5f,       time, uv));
    
    float2 distortedUV = distortUV(distortionNoise, distortUV(distortionVornoi, uv, pow(vornoiTexture, 1.2f)), gNoiseTexture);
    float4 fireDistort = shapeTexture_in.Sample(vfxSampler_in, distortedUV);
    
    float4 vornoiDiffuse = vornoiTexture_in.Sample(vfxSampler_in, UVPan(float2( 0.0f, -0.1f), 0.5f, time, uv * 0.75));
    float3 color = SampleColorRamp(colorRampTexture_in, vfxSampler_in, fireDistort);
    
    return AlphaBlend(backBuffer, fireDistort.r, (color + (vornoiDiffuse.rgb * (fireColor * fireMultiplier))));
}


// SWORDSS REQUIRES: Backbuffer, VFX_Vornoi, VFX_gNoise
float4 VFXSwordSlash(
float4 backBuffer,
float time,
float2 uv
)
{
    float4 vornoiTexture = vornoiTexture_in.Sample(vfxSampler_in, 0.5f * uv);
    float4 gNoiseTexture = noiseTexture_in.Sample(vfxSampler_in, uv);
    float4 innerGTexture = shapeTexture_in.Sample(vfxSampler_in, uv);
    float4 gMaskTexture = maskTexture_in.Sample(vfxSampler_in, uv);
    
    
    sin(time);
    
    return vornoiTexture;  //(gMaskTexture * pow(vornoiTexture, 2.0f + sin(time))).rgb, 1.0f;

}