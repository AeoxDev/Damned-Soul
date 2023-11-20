#include "RenderPipelineHeader.hlsli"

Texture2D diffuseTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D emissionTex : register(t2);
Texture2D depthTexture : register(t3);
Texture2D shadowTexture : register(t4);

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


float4 main(VS_OUT input) : SV_TARGET
{
    float4 image = diffuseTex.Sample(WrapSampler, input.uv);
    clip(image.a - 0.004f);
    return image;
}
   