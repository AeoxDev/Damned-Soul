Texture2D particleTex : register(t2);
Texture2D backbuffDepth : register(t3);
Texture2D particleDepth : register(t4);


struct PS_IN
{
    float4 position : SV_POSITION;
};


float4 main(PS_IN inval) : SV_TARGET
{
    //float4 backDepth = backbuffDepth.Load(inval.position.xyz);
    ////float4 particeDepth = particleDepth.Load(inval.position.xyz);
    
    ////if (backDepth.w > particeDepth.r)
    ////    discard;
    
    ////float4 retval = backbuffDepth.Load(inval.position.xyz);
    
    //float4 retval;
    
    //if (backbuffDepth.Load(inval.position.xyz).w >= inval.position.w + 0.0001f)
    //{
        
    //    clip(-1.0f);
    //}
    
    ////If particle nearer, load particle.
    ////float particleDepth = particleDepth.Load(inval.position.xyz);
    //float4 retval = backbuffDepth.Load(inval.position.xyz);
    ////if (retval.a == 0.f)
    ////{
    ////    clip(-1.f);
    ////}
    
    
    
    float particeDepth = particleDepth.Load(inval.position.xyz).x;
    float backDepth = backbuffDepth.Load(inval.position.xyz).x;
    
    
    if (backDepth < particeDepth)
        clip(-1.0f); 
    return particleTex.Load(inval.position.xyz);

}
