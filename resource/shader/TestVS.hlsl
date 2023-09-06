struct VS_INPUTS
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

VS_OUT main(VS_INPUTS pos)
{
	VS_OUT retval;

	retval.position = pos.position;
	retval.normal = pos.normal;
	retval.uv = pos.uv;

	return retval;
}