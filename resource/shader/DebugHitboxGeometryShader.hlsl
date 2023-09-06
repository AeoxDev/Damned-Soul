struct GSOutput
{
    float4 position : SV_POSITION;
};

cbuffer ConstantBuffer : register(b1)
{
    matrix viewAndProjection;
}

[maxvertexcount(6)]
void main(
	point float4 input[1] : SV_POSITION,
    point uint isCube[1] : IS_CUBE,
    point float widthOrRadius[1] : WIDTH,
    point float height[1] : HEIGHT,
    point float depth[1] : DEPTH,
	inout TriangleStream<GSOutput> output
)
{
    if(isCube[0])
    {
        //Create the positions of each vertice in the cube
        float4 pos[8] =
        {
            float4((input[0].x + (widthOrRadius[0] / 2.0f)), (input[0].y + (height[0] / 2.0f)), (input[0].z + (depth[0] / 2.0f)), input[0].w), //Upper Right Far
            float4((input[0].x - (widthOrRadius[0] / 2.0f)), (input[0].y + (height[0] / 2.0f)), (input[0].z + (depth[0] / 2.0f)), input[0].w), //Upper Left Far
            float4((input[0].x + (widthOrRadius[0] / 2.0f)), (input[0].y - (height[0] / 2.0f)), (input[0].z + (depth[0] / 2.0f)), input[0].w), //Lower Right Far
            float4((input[0].x - (widthOrRadius[0] / 2.0f)), (input[0].y - (height[0] / 2.0f)), (input[0].z + (depth[0] / 2.0f)), input[0].w), //Lower Left Far
            float4((input[0].x + (widthOrRadius[0] / 2.0f)), (input[0].y + (height[0] / 2.0f)), (input[0].z - (depth[0] / 2.0f)), input[0].w), //Upper Right Near
            float4((input[0].x - (widthOrRadius[0] / 2.0f)), (input[0].y + (height[0] / 2.0f)), (input[0].z - (depth[0] / 2.0f)), input[0].w), //Upper Left Near
            float4((input[0].x + (widthOrRadius[0] / 2.0f)), (input[0].y - (height[0] / 2.0f)), (input[0].z - (depth[0] / 2.0f)), input[0].w), //Lower Right Near
            float4((input[0].x - (widthOrRadius[0] / 2.0f)), (input[0].y - (height[0] / 2.0f)), (input[0].z - (depth[0] / 2.0f)), input[0].w) //Lower Left Near
        };

        GSOutput element;

	    //Multiply each vertice with the viewAndProjection matrix.
        //Far side of the cube
        element.position = mul(viewAndProjection, pos[0]); //Upper Right Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[1]); //Upper Left Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[2]); //Lower Right Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[3]); //Lower Left Far corner
        output.Append(element);

        output.RestartStrip();
        
        //Near side of the cube
        element.position = mul(viewAndProjection, pos[4]); //Upper Right Near corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[5]); //Upper Left Near corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[6]); //Lower Right Near corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[7]); //Lower Left Near corner
        output.Append(element);

        output.RestartStrip();
        
        //Upper side of the cube
        element.position = mul(viewAndProjection, pos[0]); //Upper Right Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[1]); //Upper Left Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[4]); //Upper Right Near corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[5]); //Upper Left Near corner
        output.Append(element);

        output.RestartStrip();
        
        //Lower side of the cube
        element.position = mul(viewAndProjection, pos[2]); //Lower Right Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[3]); //Lower Left Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[6]); //Lower Right Near corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[7]); //Lower Left Near corner
        output.Append(element);

        output.RestartStrip();
        
        //Right side of the cube
        element.position = mul(viewAndProjection, pos[0]); //Upper Right Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[2]); //Lower Right Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[4]); //Upper Right Near corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[6]); //Lower Right Near corner
        output.Append(element);

        output.RestartStrip();
        
        //Left side of the cube
        element.position = mul(viewAndProjection, pos[1]); //Upper Left Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[3]); //Lower Left Far corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[5]); //Upper Left Near corner
        output.Append(element);

        element.position = mul(viewAndProjection, pos[7]); //Lower Left Near corner
        output.Append(element);

        output.RestartStrip();
    }
}