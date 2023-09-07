struct Hitbox
{
    float3 middlePoint;
    uint isCube;
    float widthOrRadius;
    float height;
    float depth;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    uint isCube : IS_CUBE;
    float widthOrRadius : WIDTH;
    float height : HEIGHT;
    float depth : DEPTH;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix world;
}

StructuredBuffer<Hitbox> hitboxes : register(t0);

VertexShaderOutput main(uint vertexID : SV_VertexID)
{
    Hitbox currentHitbox = hitboxes[vertexID];
    VertexShaderOutput output;
    output.position = mul(world, float4(currentHitbox.middlePoint, 1.0f));
    output.isCube = currentHitbox.isCube;
    output.widthOrRadius = currentHitbox.widthOrRadius;
    output.height = currentHitbox.height;
    output.depth = currentHitbox.depth;

    return output;
}