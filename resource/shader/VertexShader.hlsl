cbuffer WorldMatrix : register(b0)
{
    matrix world;
}

cbuffer CameraBuffer : register(b1)
{
    float4 cameraPosition;
    matrix view;
    matrix projection;
}

struct VS_INPUTS
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

struct VS_OUT
{
    float4 position : SV_POSITION; //world, view, projection - multiplied
    float4 normal : WNORMAL; //world - multiplied
    float2 uv : UV;
    float4 camToWorldObject : CAM; // normalized 
    float4 world : WORLD;
};

VS_OUT main(VS_INPUTS pos)
{
    VS_OUT output;

    output.position = pos.position;
    output.normal = normalize(pos.normal);
    output.uv = pos.uv;
	
    output.normal = mul(pos.normal, world); //
    output.normal = normalize(output.normal);
	
    output.position = mul(pos.position, world);
    output.world = output.position;
    output.camToWorldObject = normalize(cameraPosition - output.position); //uses world position before camera perspective calculations
	
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
	
    return output;
}