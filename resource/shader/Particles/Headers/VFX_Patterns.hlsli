#include "VFX_Header.hlsli"

sampler WrapSampler : register(s3);
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
    float4 vornoiTexture = vornoiTexture_in.Sample(WrapSampler, UVPan(offsetXY_in, panSpeed_in * 1.f, time, uv * 0.5f));
    float4 gNoiseTexture = noiseTexture_in.Sample(WrapSampler, UVPan(offsetXY_in, panSpeed_in * 0.9f, time, uv));
    float4 vornoiDiffuse = vornoiTexture_in.Sample(WrapSampler, UVPan(offsetXY_in, panSpeed_in, time, uv));
    
    
    float2 distortedUV = distortUV(distortionNoise, distortUV(distortionVornoi, uv, pow(vornoiTexture, 1.2f)), gNoiseTexture);
    float4 fireDistort = shapeTexture_in.Sample(WrapSampler, distortedUV);
    float3 color = SampleColorRamp(colorRampTexture_in, WrapSampler, fireDistort);
    
    return AlphaBlend(backBuffer, fireDistort.r, color + (fireColor * fireMultiplier));
}

// ACID REQUIRES: Backbuffer, VFX_Vornoi, VFX_gNoise, VFX_FireGradient & VFX_SoftCircle
// ACID REQUIRES: VFX_Vornoi, VFX_gNoise, VFX_FireGradient & VFX_SoftCircle_Nomore
float4 VFXAcidSpit(
in float4 backBuffer,
in float time,
in float2 uv
)
{
    // To change the speed and direction of the rotations, change what time is multiplied with.   
    float4 distortVornoi = vornoiTexture_in.Sample(WrapSampler, UVRotate(uv, time * -15.6f, 0.8f));
    float2 vornoiUV = distortUV(0.1f, uv, distortVornoi);
    
    float4 panningVornoi = vornoiTexture_in.Sample(WrapSampler, UVRotate(uv, time * 15.6f, 0.65f));
    
    float4 distortedProjectile = shapeTexture_in.Sample(WrapSampler, vornoiUV);
    float4 newProjectile = distortedProjectile - (panningVornoi * distortedProjectile);
    
    float4 colorizeProjectile = lerp((newProjectile - float4(0.0f, 0.0f, 1.05f, 1.0f)), (newProjectile - float4(1.0f, 0.0f, 1.0f, 1.0f)), 0.18f);
    
    return AlphaBlend(backBuffer, distortedProjectile.r, colorizeProjectile.rgb);
}

// REQUIRES: VFX_Vornoi, VFX_CircleSoft_Nomore
float4 VFXAcidGround(
in float4 backBuffer,
in float time,
in float2 uv
)
{
// Creates the Texture Patterns and Colorizes them using subtract.
    float4 panVornoi = (float4(0.0f, 1.0f, 0.0f, 1.0f) - SamplePolarCoordinateTexture(vornoiTexture_in, WrapSampler, time, uv, 0.0f, -0.05f, 0.2f));
    float4 rotateVornoi = (float4(1.0f, 1.0f, 0.0f, 1.0f) * SamplePolarCoordinateTexture(vornoiTexture_in, WrapSampler, time, uv, 0.15f, -0.01f, 0.3f));
    float4 colorizedAcid = lerp(panVornoi, rotateVornoi, 0.75f);
    
// Creates the Alpha Mask
    float alphaMask = shapeTexture_in.Sample(WrapSampler, distortUV(0.12f, uv, vornoiTexture_in.Sample(WrapSampler, uv))).r;
    alphaMask = clamp(2.0f * alphaMask, 0.0f, 1.0f);
    
 // Alpha Blends
    return AlphaBlend(backBuffer, alphaMask, colorizedAcid.rgb);
}


// REQUIRES: VFX_SoftCircle_Nomore, VFX_Vornoi, VFX_gNoise
float4 VFXFireBall(
in float4 backBuffer,
in float time,
in float2 uv
)
{
    // Pan the distortion Textures
    float panGNoise = noiseTexture_in.Sample(WrapSampler, UVRotate(uv, time * 15.6f, 0.8f)).r;
    float panVornoi = vornoiTexture_in.Sample(WrapSampler, UVRotate(uv, time * -5.0f, 0.5f)).r;
    
    // Distort UV channels
    float2 fireUV = lerp(lerp(panGNoise, panVornoi, -0.31f), uv, 0.9f);
    
    // Create Alpha Mask
    float alphaMask = shapeTexture_in.Sample(WrapSampler, fireUV).r;
    
    // Mask magic
    float4 subtractionMask = (alphaMask - (alphaMask * panVornoi)) * float4(1.0f, 0.6f, 0.0f, 0.0f);
    float4 multiplicationMask = (alphaMask - (alphaMask * (panVornoi * 0.49f))) - float4(0.0f, 0.43f, 1.0f, 1.0f);
    
    // Colors the projectile
    float4 colorizedProjectile = subtractionMask + multiplicationMask;
    
    return AlphaBlend(backBuffer, alphaMask, colorizedProjectile.rgb);
}

// SWORDSS REQUIRES: Backbuffer, VFX_Vornoi, VFX_gNoise
float4 VFXSwordSlash(
float4 backBuffer,
float time,
float2 uv
)
{
    float4 vornoiTexture = vornoiTexture_in.Sample(WrapSampler, 0.5f * uv);
    float4 gNoiseTexture = noiseTexture_in.Sample(WrapSampler, uv);
    float4 innerGTexture = shapeTexture_in.Sample(WrapSampler, uv);
    float4 gMaskTexture = maskTexture_in.Sample(WrapSampler, uv);
    
    
    sin(time);
    
    return vornoiTexture;  //(gMaskTexture * pow(vornoiTexture, 2.0f + sin(time))).rgb, 1.0f;

}