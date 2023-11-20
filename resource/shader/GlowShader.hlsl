#include "RenderPipelineHeader.hlsli"	// This right?

Texture2D emissionTex : register(t2);
SamplerState WrapSampler : register(s0);

float4 main(VS_OUT input) : SV_TARGET
{
    float4 in_rgba = emissionTex.Sample(WrapSampler, input.uv);
    if (in_rgba.r <= 0.0f && in_rgba.g <= 0.0f && in_rgba.b <= 0.0f)
    {
        // Apply color from glow component
        return float4(in_rgba.rgb, 0.f);
    }
    
    return in_rgba;
    //return color * color.a;
    
    // IMPORTANT: Currently, everything needs a glow component.
}