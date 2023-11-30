#include "RenderPipelineHeader.hlsli"

float4 main(VS_OUT input) : SV_TARGET
{
    //return float4((input.normal.xyz + 1.f) * .5f, 1.0f);
    return float4(1.f, 1.f, 1.f, 1);
}