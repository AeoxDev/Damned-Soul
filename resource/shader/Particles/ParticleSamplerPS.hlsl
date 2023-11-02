Texture2D particleTex : register(t3);


struct PS_IN
{
    float4 position : SV_POSITION;
};


float4 main(PS_IN inval) : SV_TARGET
{
    float4 retval = particleTex.Load(inval.position.xyz);
    
    if (retval.a == 0.f)
    {
        clip(-1.f);
    }
    
    return retval;

}
