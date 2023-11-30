Texture2D particleTex : register(t2);
Texture2D backbuffDepth : register(t3);
Texture2D particleDepth : register(t4);



struct PS_IN
{
    float4 position : SV_POSITION;
};


float4 main(PS_IN inval) : SV_TARGET
{
    float4 backDepth = backbuffDepth.Load(inval.position.xyz);
    float4 particeDepth = particleDepth.Load(inval.position.xyz);
    
    if (backDepth.w > particeDepth.r)
        discard;
    
    float4 retval = particleTex.Load(inval.position.xyz);
    
    
    
    if (retval.a == 0.f)
    {
        clip(-1.f);
    }
    
    return retval;

}
