Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D emissionTex : register(t2);

SamplerState WrapSampler : register(s0);


cbuffer LightingBuffer : register(b0)
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float intensity;
}


struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	uint material : MATERIAL;
	float2 uv : UV;
};




float4 main(VS_OUT input) : SV_TARGET
{
	return albedoTex.Sample(WrapSampler, input.uv);
}