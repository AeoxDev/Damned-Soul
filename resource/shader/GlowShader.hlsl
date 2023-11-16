#include "RenderPipelineHeader.hlsli"	// This right?

Texture2D emissionTex : register(t2);
SamplerState WrapSampler : register(s0);

float4 main(VS_OUT input) : SV_TARGET
{
    return emissionTex.Sample(WrapSampler, input.uv);
}