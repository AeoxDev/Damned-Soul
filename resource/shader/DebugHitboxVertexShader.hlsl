struct VisualHitbox
{
    float4 color; //4
    float3 middlePoint; //3
    float radius; //1
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float radius : RADIUS;
};

//cbuffer ConstantBuffer : register(b0)
//{
//    matrix world;
//}

StructuredBuffer<VisualHitbox> hitboxes : register(t0);

VertexShaderOutput main(uint vertexID : SV_VertexID)
{
    VisualHitbox currentHitbox = hitboxes[vertexID];
    VertexShaderOutput output;
    output.position = mul(float4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), float4(currentHitbox.middlePoint, 1.0f));
    output.color = currentHitbox.color;
    output.radius = currentHitbox.radius - 0.49f;

    return output;
}