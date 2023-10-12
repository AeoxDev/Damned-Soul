SamplerState wrapSampler : register(s0);
Texture2D UITexture : register(t0);

struct PS_INPUTS
{
    float4 position : SV_POSITION;
    float4 screenPos : SCREEN_POS;
};


float4 main(PS_INPUTS input) : SV_TARGET
{
    float4 pos = input.screenPos;
    //pos.xyz /= pos.w;
    //const float2 tx = float2(0.5f * pos.x + 0.5f, -0.5f * pos.y + 0.5f);
    //const float4 sample = UITexture.Sample(wrapSampler, tx);
    const float4 load = UITexture.Load(int3(input.position.r, input.position.g, 0));
    if (load.a == 0)
        discard;
    
    return load;
}