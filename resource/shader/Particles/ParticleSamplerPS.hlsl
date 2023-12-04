Texture2D particleTex : register(t2);
Texture2D backbuffDepth : register(t3);
Texture2D particleDepth : register(t4);


struct PS_IN
{
    float4 position : SV_POSITION;
};


float4 main(PS_IN inval) : SV_TARGET
{    
    float particeDepth = particleDepth.Load(inval.position.xyz).x;
    float backDepth = backbuffDepth.Load(inval.position.xyz).x;
    
    
    if (backDepth < particeDepth)
        clip(-1.0f);
    return particleTex.Load(inval.position.xyz);
}
