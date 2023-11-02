struct VS_IN
{
    float3 position : POSITION;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
};

VS_OUT main(VS_IN inval)
{
    VS_OUT retval;
    retval.position = float4(inval.position.x, inval.position.y, inval.position.z, 1.0f);

    return retval;
}

