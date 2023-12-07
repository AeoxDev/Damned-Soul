#include "RenderPipelineHeader.hlsli"

cbuffer shatter : register(b2)
{
    float time;
    float strength;
};


[maxvertexcount(3)]
void main(triangle VS_OUT input[3], inout TriangleStream<GS_OUT> output)
{
    matrix viewProj = mul(view, projection);
    float4 midPoint = (input[0].world + input[1].world + input[2].world) / 3;
    float3 faceNormal = normalize((input[0].normal.xyz + input[1].normal.xyz + input[2].normal.xyz) / 3);
    float3 direction;
    float usedTime = time;
    direction = faceNormal;
    
    //if (useOrigin)
    //{
    //    float3 OtoT = midPoint.xyz - origin.xyz;
    //    float length = sqrt(pow(OtoT.x, 2) + pow(OtoT.y, 2) + pow(OtoT.z, 2));
    //    //usedTime -= length * 0.1f;
    //    //if (usedTime < 0)
    //    //{
    //    //    usedTime = 0;
    //    //}
    //    direction = normalize(OtoT);
    //}
    //else
    //{
    //    direction = faceNormal;
    //}
    
    float resistance = 0.1f;
    float gravStrength = 9.8f;

    float nullTime = ((((gravStrength / -direction.y) - strength) / (2 * resistance)) + abs(sqrt(midPoint.y / (direction.y * resistance)) + pow(((gravStrength / -direction.y) - strength) / (2 * resistance), 2)));
    if (nullTime > usedTime)
    {
        nullTime = usedTime;
    }
    
    float yCorrection = 0.1;
    if (midPoint.y < 0)
    {
        nullTime = 0;
        yCorrection += -midPoint.y;
    }
    
    float x = (direction.x * ((strength * nullTime) - (resistance * pow(nullTime, 2))));
    float y = (direction.y * ((strength * nullTime) - (resistance * pow(nullTime, 2)))) - (nullTime * gravStrength) + yCorrection;
    float z = (direction.z * ((strength * nullTime) - (resistance * pow(nullTime, 2))));
    float3 displacement = float3(x, y, z);

    GS_OUT p1;
    GS_OUT p2;
    GS_OUT p3;
    GS_OUT p4;
    
    p1.base = input[0];
    p2.base = input[1];
    p3.base = input[2];
    p4.base = input[0];
    
    p1.base.position = mul(input[0].world + float4(displacement, 0), viewProj);
    p2.base.position = mul(input[1].world + float4(displacement, 0), viewProj);
    p3.base.position = mul(input[2].world + float4(displacement, 0), viewProj);
    p4.base.position = mul(midPoint - float4(faceNormal, 0) + float4(displacement, 0), viewProj);
    
    p1.base.world = input[0].world + float4(displacement, 0);
    p2.base.world = input[1].world + float4(displacement, 0);
    p3.base.world = input[2].world + float4(displacement, 0);
    p4.base.world = midPoint - float4(faceNormal * 100, 0) + float4(displacement, 0);
    
    p1.base.normal = input[0].normal;
    p2.base.normal = input[1].normal;
    p3.base.normal = input[2].normal;
    p4.base.normal = -float4(faceNormal, 0);
    
    p1.base.uv = input[0].uv;
    p2.base.uv = input[1].uv;
    p3.base.uv = input[2].uv;
    p4.base.uv = (input[0].uv + input[1].uv + input[2].uv) / 3;
    
    //Herman code
    float3 edge1, edge2;
    // Delta of U and V values between index 0 and 1 and 0 and 2
    float delta_u1, delta_u2, delta_v1, delta_v2;
    
	// Set directions from Vertex 0
    edge1 = input[1].position.xyz - input[0].position.xyz;
    edge2 = input[2].position.xyz - input[0].position.xyz;
    
	// Set U advancements from Vertex 0
    delta_u1 = input[1].uv.x - input[0].uv.x;
    delta_u2 = input[2].uv.x - input[0].uv.x;
    
	// Set V advancements from Vertex 0
    delta_v1 = input[1].uv.y - input[0].uv.y;
    delta_v2 = input[2].uv.y - input[0].uv.y;

    float2x2 inverseDUV = { delta_v2, -delta_v1, -delta_u2, delta_u1 };
    inverseDUV /= delta_u1 * delta_v2 - delta_v1 * delta_u2;

    float2x3 directions = { edge1, edge2 };

    float2x3 TBMatrix = mul(inverseDUV, directions);
	
    p1.tbn = float3x3(TBMatrix[0], cross(TBMatrix[0], input[0].normal.xyz), input[0].normal.xyz);
    p2.tbn = float3x3(TBMatrix[0], cross(TBMatrix[0], input[1].normal.xyz), input[1].normal.xyz);
    p3.tbn = float3x3(TBMatrix[0], cross(TBMatrix[0], input[2].normal.xyz), input[2].normal.xyz);
    p4.tbn = (p1.tbn + p2.tbn + p3.tbn) / 3;
    
    output.Append(p1);
    output.Append(p2);
    output.Append(p3);
    output.RestartStrip();
    
    output.Append(p1);
    output.Append(p3);
    output.Append(p4);
    output.RestartStrip();
    
    output.Append(p2);
    output.Append(p4);
    output.Append(p3);
    output.RestartStrip();
    
    output.Append(p1);
    output.Append(p4);
    output.Append(p2);
    output.RestartStrip();
}