#include "RenderPipelineHeader.hlsli"


[maxvertexcount(3)]
void main(
	// Vertex Shader OUtput
	triangle VS_OUT input[3], 
	inout TriangleStream<GS_OUT> output
)
{
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
	
	for (uint i = 0; i < 3; i++)
	{
		// One of the vertices
        GS_OUT element;
		// Simply set the values
        element.base = input[i];
        element.tbn = float3x3(TBMatrix[0], cross(TBMatrix[0], input[i].normal.xyz), input[i].normal.xyz);
		output.Append(element);
	}
}