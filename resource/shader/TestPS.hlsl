struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

float4 main(VS_OUT input) : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);//input.position;
}