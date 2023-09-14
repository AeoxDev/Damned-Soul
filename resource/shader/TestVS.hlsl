cbuffer WorldMatrix : register (b0)
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
	uint material : MATERIAL;
	float2 uv : UV;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	uint material : MATERIAL;
	float2 uv : UV;
};

VS_OUT main(VS_INPUTS pos)
{
	VS_OUT retval;

	retval.position = pos.position;
	retval.normal = pos.normal;
	retval.uv = pos.uv;

	retval.material = pos.material;
	
	retval.position = mul(pos.position, world);
    retval.position = mul(retval.position, view);
    retval.position = mul(retval.position, projection);

	return retval;
}