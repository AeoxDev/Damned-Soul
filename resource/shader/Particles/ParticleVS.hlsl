#include "ParticleHeader.hlsli"

cbuffer WorldMatrix : register(b0)
{
    matrix world;
}


cbuffer metadataBuffer : register(b2)
{
    int start;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 rgb : RGB;
    float rotationZ : ROTATIONZ;
    float size : SIZE;
};

StructuredBuffer<Input> particles : register(t2);


VS_OUT main(uint vertexID : SV_VertexID)
{
    Input inval = particles[vertexID + start];
    //inval = particles[instanceID];
    VS_OUT retval;
    
    retval.position = float4(inval.position, 1.f);
    retval.worldPosition = mul(retval.position, world);
    retval.rgb = float4(inval.rgb, 1.f);
    retval.rotationZ = inval.rotationZ;
    retval.size = inval.size;

    return retval;
}