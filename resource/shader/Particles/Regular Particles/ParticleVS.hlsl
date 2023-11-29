#include "../Headers/ParticleHeader.hlsli"

cbuffer startBuff : register(b2)
{
    int startIndex;
};

cbuffer WorldMatrix : register(b0)
{
    matrix world;
}

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 rgb : RGB;
    float rotationZ : ROTATIONZ;        //!!! is currently used to define pattern in PS-Shader for flipAnimations
    float size : SIZE;
    float time : TIME;
    int patterns : PATTERNS;
    int VFXpatterns : VFXPATTERNS;
};

StructuredBuffer<Input> particles : register(t0);

VS_OUT main(uint vertexID : SV_VertexID)
{
    int index = (int) vertexID + startIndex;
    Input inval = particles[index];
    //inval = particles[instanceID];
    VS_OUT retval;
    
    retval.position = float4(inval.position, 1.f);
    retval.worldPosition = mul(retval.position, world);
    retval.rgb = float4(inval.rgb, 1.f);
    retval.rotationZ = inval.rotationZ; //!!! is currently used to define pattern in PS-Shader for flipAnimations
    retval.size = inval.size;
    retval.time = inval.time;
    retval.patterns = (int)inval.patterns;
    retval.VFXpatterns = (int) inval.VFXpatterns;
    return retval;
}