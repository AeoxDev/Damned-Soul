#include "RenderPipelineHeader.hlsli"


float main(VS_OUT input) : SV_Depth
{
    return input.depth.x / input.depth.y;
}