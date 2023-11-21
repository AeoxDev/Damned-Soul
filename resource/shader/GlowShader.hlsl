#include "RenderPipelineHeader.hlsli"

Texture2D emissionTex : register(t2);
SamplerState WrapSampler : register(s0);

// Decides the color of glow effect for this entity.
cbuffer GlowInfo : register(b2)
{
    float in_r;
    float in_g;
    float in_b;
}

float4 main(VS_OUT input) : SV_TARGET
{
    float4 tex_rgba = emissionTex.Sample(WrapSampler, input.uv);
    if (tex_rgba.r <= 0.0f && tex_rgba.g <= 0.0f && tex_rgba.b <= 0.0f)
    {
        // Apply color from glow component.
        float4 glow_col = float4(in_r, in_g, in_b, 0.f);
        return glow_col;
    }
    
    // Calculate alpha.
    float avg = (tex_rgba.r + tex_rgba.g + tex_rgba.b) / 3;
    float4 glow_col = float4(in_r, in_g, in_b, avg);
    
    return glow_col;
    
    // IMPORTANT: Currently, everything needs a glow component! Otherwise, glow only appears inside an object's outline.
    // TODO: Fact-check this.
}