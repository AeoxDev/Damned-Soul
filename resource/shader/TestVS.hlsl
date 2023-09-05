struct VS_INPUTS
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}