#include "RenderPipelineHeader.hlsli"

cbuffer level : register(b3)
{
    int level;
}

VS_OUT main(VS_INPUTS pos)
{
    VS_OUT output = (VS_OUT) 0;

    output.position = pos.position;
    output.normal = normalize(pos.normal);
    output.uv.x = pos.uv.x;
    // 17 is the max amount of levels
    // Taking into consideration 9 Levels and 8 Shops
    output.uv.y = pos.uv.y + (1.f / 17.f) * level;
	
    output.normal = normalize(mul(pos.normal, worldNormal)); //
	
    output.world = mul(pos.position, world);
    output.camToWorldObject = normalize(cameraPosition - output.position); //uses world position before camera perspective calculations
    
    matrix WVP = mul(world, view);
    WVP = mul(WVP, projection);

    
    output.depth.x = output.position.z;
    output.depth.y = output.position.w;
    return output;
}