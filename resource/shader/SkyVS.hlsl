#include "RenderPipelineHeader.hlsli"

VS_OUT main(VS_INPUTS pos)
{
    VS_OUT output = (VS_OUT)0;

    //output.position = pos.position;v 
    output.normal = normalize(pos.normal);
    output.uv = pos.uv;
	
    output.normal = normalize(mul(pos.normal, worldNormal)); //
	
    output.world = mul(pos.position, world);
    output.camToWorldObject = normalize(cameraPosition - output.position); //uses world position before camera perspective calculations
	
    
    matrix WVP = mul(world, view);
    WVP = mul(WVP, projection);

    output.position = mul(pos.position, WVP).xyzw;
    
    output.depth.x = output.position.z;
    output.depth.y = output.position.w;
    return output;
}