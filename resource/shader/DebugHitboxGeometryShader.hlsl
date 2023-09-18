struct GSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

//cbuffer ConstantBuffer : register(b0)
//{
//    matrix viewAndProjection;
//}

cbuffer CameraBuffer : register(b0)
{
    float4 cameraPosition;
    matrix view;
    matrix projection;
}

[maxvertexcount(9)]
void main(
	point float4 pos[1] : SV_POSITION,
    point float4 color[1] : COLOR,
    point float radius[1] : RADIUS,
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element;
    
    for (int i = 0; i < 9; i++)
    {
        float angle = i * 360.0f / 8.0f; //Angle for each vertice in the octagon
        float4 corner = float4(pos[0].x + radius[0] * cos(radians(angle)), pos[0].y, pos[0].z + radius[0] * sin(radians(angle)), 1.0f); //Change to Z when fixed
        element.position = mul(corner, view);
        element.position = mul(element.position, projection);
        element.color = color[0];
        output.Append(element);
    }
    
    output.RestartStrip();
}