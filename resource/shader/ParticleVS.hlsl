cbuffer WorldMatrix : register(b0)
{
    matrix world;
}

struct VS_IN
{
    float3 position : POSITION;
    float3 velocity : VELOCITY;
    float3 rgb : RBG;
};

struct VS_OUT
{
    float4 worldPosition : SV_POSITION;
    float3 velocity : VELOCITY;
    float4 rbg : RBG;
};

VS_OUT main(VS_IN inval)
{
    VS_OUT retval;
    
    retval.worldPosition = mul(float4(inval.position, 1.f), world);

    
    retval.velocity = inval.velocity;
    retval.rbg = float4(inval.rgb, 1.f);

    return retval;
}