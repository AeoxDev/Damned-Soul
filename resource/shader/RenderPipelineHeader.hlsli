#ifndef RENDER_HEADER
#define RENDER_HEADER


/*BASE INFO*/
cbuffer WorldMatrix : register(b0)
{
    matrix world;
    matrix worldNormal;

}

cbuffer CameraBuffer : register(b1)
{
    float4 cameraPosition;
    matrix view;
    matrix projection;
    matrix shadowView;
    matrix shadowProjection;
}

struct VS_INPUTS
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

struct VS_OUT //VS_OUT needs to be exaxtly the same in all vertexshaders
{
    float4 position : SV_POSITION; //world, view, projection - multiplyed
    float4 normal : WNORMAL; //world - multiplied
    float2 uv : UV;
    float4 camToWorldObject : CAM; // normalized 
    float4 world : WORLD;
    float2 depth : DEPTH;
};

struct GS_OUT
{
    VS_OUT base;
    // Tangent, Bitangent, Normal
    float3x3 tbn : TBN;
};
/*BASE INFO*/

/*SKELETON INFO*/
// Structured buffer of bones
StructuredBuffer<matrix> skeleMath : register(t0);
// Structured buffer of bones for normals
StructuredBuffer<matrix> skeleMathNormal : register(t1);

struct VS_INPUTS_SKELETON
{
    VS_INPUTS base;
    uint4 bIdx : BONE_INDEX;
    float4 bWeight : BONE_WIEGHT;
};
/*SKELETON INFO*/

#endif