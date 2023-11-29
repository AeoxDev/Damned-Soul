#include "RenderPipelineHeader.hlsli"

Texture2D diffuseTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D emissionTex : register(t2);
Texture2D depthTexture : register(t3);
Texture2D shadowTexture : register(t4);
Texture2D backbufferTexture_in : register(t5);


SamplerState WrapSampler : register(s0);
SamplerState ShadowClampSampler : register(s1);

#define LIGHT_COMPONENT_ARRAY_LIMIT 64

struct LightComponent
{
    int type; // 1 pointlight, 2 spotlight
    float fallofFactor; //attenuation
    float lightRange;
    float lightCone;
    float4 lightColor;
    float4 lightDirection;
    float4 lightPosition;
};


cbuffer LightComponentShaderBuffer : register(b2)
{
    int firstLight; //First light in array
    int lastLight; //Last light in array
    float gammaCorrection; //Gamma correction
    float padding2;
    float4 dirLightColor;
    float4 dirLightDirection;
    float4 colorMultiplier;
    float4 colorAdditive;
    LightComponent lights[LIGHT_COMPONENT_ARRAY_LIMIT];
};

// TO DO: FIX THIS
#define SCREEN_X 1600
#define SCREEN_Y 900


// Backbuffer Sampling for the UV of the particle
float4 SampleBackbuffer(in float4 position, in float2 screenResolution, in Texture2D backBufferTexture, in sampler samplerUsed)
{
    float2 screenSpaceUV = float2((position.x / screenResolution.x), (position.y / screenResolution.y));
    return backBufferTexture.Sample(samplerUsed, screenSpaceUV);
}

// Alpha blending with the backbuffer. Note that the "diffuse texture" can be manipulated per color channel, and could also just be a mask.
float4 AlphaBlend(in float4 backBuffer, in float alphaMaskOrValue, in float3 diffuseTexture, in float3 colorValue = float3(1.0f, 1.0f, 1.0f))
{
    float4 blendResults =
    {
        (backBuffer.r * (1 - alphaMaskOrValue.r) + (diffuseTexture.r * colorValue.r * alphaMaskOrValue.r)),
        (backBuffer.g * (1 - alphaMaskOrValue.r) + (diffuseTexture.g * colorValue.g * alphaMaskOrValue.r)),
        (backBuffer.b * (1 - alphaMaskOrValue.r) + (diffuseTexture.b * colorValue.b * alphaMaskOrValue.r)),
        1.0f
    };

    return float4(saturate(blendResults.rgb), 1.0f);
}

float4 main(VS_OUT input) : SV_TARGET
{
    float2 screenRes = float2(SCREEN_X, SCREEN_Y);
    float4 backbuffer = SampleBackbuffer(input.position, screenRes, backbufferTexture_in, WrapSampler);
    
    AlphaBlend(backbuffer, )
    
    float4 image = diffuseTex.Sample(WrapSampler, input.uv);
    clip(image.a - 0.004f);
    return image;
}
   