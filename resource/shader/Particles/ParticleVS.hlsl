cbuffer WorldMatrix : register(b1)
{
    matrix world;
}

struct VS_IN
{
    float3 position : POSITION;
    float time : TIME;
    float3 velocity : VELOCITY;
    float rotationZ : ROTATIONZ;
    float3 rgb : RGB; // Red Green Blue
    float size : SIZE;

};

struct VS_OUT
{
    float4 worldPosition : SV_POSITION;
    float4 rbg : RBG;
    float rotationZ : ROTATIONZ;
    float size : SIZE;
};

VS_OUT main(VS_IN inval)
{
    VS_OUT retval;
    
    retval.worldPosition = mul(float4(inval.position, 1.f), world);
    retval.rbg = float4(inval.rgb, 1.f);
    retval.rotationZ = inval.rotationZ;
    retval.size = inval.size;

    return retval;
}