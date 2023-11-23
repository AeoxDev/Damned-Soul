cbuffer WorldMatrix : register(b0)
{
    matrix world;
}

struct VS_IN
{
    float3 position : POSITION;
    float time : TIME;
    float3 velocity : VELOCITY;
    float rotationZ : ROTATIONZ;        //!!! is currently used to define pattern in PS-Shader for flipAnimations
    float3 rgb : RGB; // Red Green Blue
    float size : SIZE;
    int patterns : PATTERNS;

};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 rgb : RGB;
    float rotationZ : ROTATIONZ;        //!!! is currently used to define pattern in PS-Shader for flipAnimations
    float size : SIZE;
    float time : TIME;
    int patterns : PATTERNS;
};

VS_OUT main(VS_IN inval)
{
    VS_OUT retval;
    
    retval.position = float4(inval.position, 1.f);
    retval.worldPosition = mul(retval.position, world);
    retval.rgb = float4(inval.rgb, 1.f);
    retval.rotationZ = inval.rotationZ; //!!! is currently used to define pattern in PS-Shader for flipAnimations
    retval.size = inval.size;
    retval.time = inval.time;
    retval.patterns = (int)inval.patterns;
    return retval;
}