#include "RenderPipelineHeader.hlsli"

VS_OUT main(VS_INPUTS pos)
{
    VS_OUT output;

    output.position = pos.position;
    output.normal = normalize(pos.normal);
    output.uv = pos.uv;
	
    output.normal = normalize(mul(pos.normal, worldNormal)); //
	
    output.position = mul(pos.position, world);
    output.world = output.position;
    output.camToWorldObject = normalize(cameraPosition - output.position); //uses world position before camera perspective calculations
	
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
	
    return output;
}