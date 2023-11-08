cbuffer WorldMatrix : register(b0)
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
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 rgb : RGB;
    float rotationZ : ROTATIONZ;
    float size : SIZE;
};

StructuredBuffer<VS_IN> particles : register(t2);


VS_OUT main(/*VS_IN inval*/ uint vertexID : SV_VertexID)
{
    VS_IN inval = particles[vertexID];
    VS_OUT retval;
    
    retval.position = float4(inval.position, 1.f);
    retval.worldPosition = mul(retval.position, world);
    retval.rgb = float4(inval.rgb, 1.f);
    retval.rotationZ = inval.rotationZ;
    retval.size = inval.size;

    return retval;
}