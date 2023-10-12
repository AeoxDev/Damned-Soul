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
	float4 svPosition : SV_POSITION;
    float4 position : POSITION;
};

VS_OUT main(VS_INPUTS pos)
{
	VS_OUT retval;

    retval.position = mul(pos.position, world);
	
    retval.position = mul(pos.position, world);
    retval.position = mul(retval.position, view);
	
    retval.svPosition = mul(pos.position, world);
    retval.svPosition = mul(retval.svPosition, view);
    retval.svPosition = mul(retval.svPosition, projection);

	return retval;
}