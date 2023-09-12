struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

float4 main(VS_OUT input) : SV_TARGET
{
	return (input.normal + 1) * 0.5f;
}