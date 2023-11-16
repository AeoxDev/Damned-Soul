#include "RenderPipelineHeader.hlsli"
struct SimpleVertex
{
    float4 position;
    float4 color;
};

struct SimpleModel
{
    SimpleVertex vertices[8];
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer ModelData : register(b2)
{
    SimpleModel debugModel;
}
//cbuffer ConstantBuffer : register(b0)
//{
//    matrix world;
//}

VertexShaderOutput main(uint vertexID : SV_VertexID)
{
    VertexShaderOutput output;
    output.position = mul(debugModel.vertices[vertexID].position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.color = debugModel.vertices[vertexID].color;

    return output;
}