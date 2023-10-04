cbuffer CameraBuffer : register(b0)
{
    float4 cameraPosition;
    matrix view;
    matrix projection;
}

struct GS_IN
{
    float4 position : SV_POSITION;
    float4 rbg : RBG;
    float rotationZ : ROTATIONZ;
    float size : SIZE;
};

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rbg : RBG;
};

[maxvertexcount(6)]
void main(
	point GS_IN inval[1], 
	inout TriangleStream< GS_OUT > retval
)
{
    float3 look = cameraPosition.xyz - inval[0].position.xyz;
    //look.y = 0.f;
    look = normalize(look);
    
    
    // TODO: Up vector needs to be calculated independantly
    float3 up = float3(0.f, 1.f, 0.f);
    float3 right = cross(up, look);
    
    
    
    // Now construct the billboard, starting with positions
    float3 vertices[6];
    vertices[0] = inval[0].position.xyz - right * inval[0].size - up * inval[0].size; // Get bottom left vertex
    vertices[1] = inval[0].position.xyz - right * inval[0].size + up * inval[0].size; // Get top left vertex
    vertices[2] = inval[0].position.xyz + right * inval[0].size - up * inval[0].size; // Get bottom right vertex
    vertices[3] = inval[0].position.xyz + right * inval[0].size - up * inval[0].size; // Get bottom right vertex
    vertices[4] = inval[0].position.xyz - right * inval[0].size + up * inval[0].size; // Get top left vertex
    vertices[5] = inval[0].position.xyz + right * inval[0].size + up * inval[0].size; // Get top right vertex

    float4x4 viewProj = mul(view, projection);

    // Now append it to output
    GS_OUT retappend;
    [unroll]
    for (int i = 0; i < 6; i++)
    {
        retappend.position = mul(float4(vertices[i], 1.f), viewProj); // times rotationZ here?
        // INSERT rotate on Z-axis
        retappend.position = retappend.position / retappend.position.w;
        retappend.rbg = inval[0].rbg;


        retval.Append(retappend);

        if (i == 2)
            retval.RestartStrip();
    }
}