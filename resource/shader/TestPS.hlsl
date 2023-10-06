Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D emissionTex : register(t2);

SamplerState WrapSampler : register(s0);





struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
	uint4 index : INDEX;
	float4 weight : WEIGHT;
};




float4 main(VS_OUT input) : SV_TARGET
{
	return albedoTex.Sample(WrapSampler, input.uv);

	for (int i = 0; i < 4; ++i)
	{
		if (2 == input.index[i])
		return (input.weight[i]) * (input.weight[i]);
	}

	return 0;
}
   
    