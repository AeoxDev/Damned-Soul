#include "RenderPipelineHeader.hlsli"


float4 main(VS_OUT input) : SV_TARGET
{
    float depth = input.depth.x / input.depth.y;
    return float4(depth, depth, depth, depth);
}