#include "RenderPipelineHeader.hlsli"

float main(VS_OUT input) : SV_TARGET
{
    return input.depth.x / input.depth.y;
}