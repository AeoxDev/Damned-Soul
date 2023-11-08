cbuffer CameraBuffer : register(b1)
{
    float4 cameraPosition;
    matrix view;
    matrix projection;
}

struct GS_IN
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 rgb : RGB;
    float rotationZ : ROTATIONZ;
    float size : SIZE;
    float time : TIME;
    float4 patterns : PATTERNS;
};

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV;
    float time : TIME;
    float4 patterns : PATTERNS;
};

[maxvertexcount(6)]
void main(
	point GS_IN inval[1], 
	inout TriangleStream< GS_OUT > retval
)
{
    float3 look = cameraPosition.xyz - inval[0].worldPosition.xyz;
    //look.y = 0.f;
    look = normalize(look);
    
    
    // TODO: Up vector needs to be calculated independantly
    float3 up = float3(0.f, 1.f, 0.f);
    float3 right = cross(up, look);
    
    
    
    // Now construct the billboard, starting with positions
    float3 vertices[6];
    float2 uvCord[6];
    
    // Get bottom left vertex
    vertices[0] = inval[0].worldPosition.xyz - right * (inval[0].size / 2) - up * (inval[0].size / 2);
    //uvCord[0] = float2(0.0f, 1.0f);
    uvCord[0] = float2(1.0f, 1.0f);
    
    // Get top left vertex
    vertices[1] = inval[0].worldPosition.xyz - right * (inval[0].size / 2) + up * (inval[0].size / 2);
    //uvCord[1] = float2(0.0f, 0.0f);
    uvCord[1] = float2(1.0f, 0.0f);
    
    // Get bottom right vertex
    vertices[2] = inval[0].worldPosition.xyz + right * (inval[0].size / 2) - up * (inval[0].size / 2);
    //uvCord[2] = float2(1.0f, 1.0f);
    uvCord[2] = float2(0.0f, 1.0f);
    
    // Get bottom right vertex
    vertices[3] = inval[0].worldPosition.xyz + right * (inval[0].size / 2) - up * (inval[0].size / 2);
    //uvCord[3] = float2(1.0f, 1.0f);
    uvCord[3] = float2(0.0f, 1.0f);
    
    // Get top left vertex
    vertices[4] = inval[0].worldPosition.xyz - right * (inval[0].size / 2) + up * (inval[0].size / 2);
    //uvCord[4] = float2(0.0f, 0.0f);
    uvCord[4] = float2(1.0f, 0.0f);
    
    // Get top right vertex
    vertices[5] = inval[0].worldPosition.xyz + right * (inval[0].size / 2) + up * (inval[0].size / 2);
    //uvCord[5] = float2(1.0f, 0.0f);
    uvCord[5] = float2(0.0f, 0.0f);
    
    float4x4 viewProj = mul(view, projection);

    // Now append it to output
    GS_OUT retappend;
    [unroll]
    for (int i = 0; i < 6; i++)
    {
        retappend.position = mul(float4(vertices[i].xyz, inval[0].worldPosition.w), viewProj); // times rotationZ here?
        // INSERT rotate on Z-axis
        retappend.position = retappend.position / retappend.position.w;
        retappend.rgb = inval[0].rgb;
        retappend.uv = uvCord[i];
        retappend.time = inval[0].time;
        retappend.patterns = inval[0].patterns;

        retval.Append(retappend);

        if (i == 2)
            retval.RestartStrip();
    }
}