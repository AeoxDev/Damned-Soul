Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D emissionTex : register(t2);

SamplerState ClampSampler : register(s0);

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
};



float4 main(VS_OUT input) : SV_TARGET
{
	return albedoTex.Sample(ClampSampler, input.uv);
}