cbuffer WorldMatrix : register(b0)
{
    matrix world;
}

cbuffer CameraBuffer : register(b1)
{
    float4 cameraPosition;
    matrix view;
    matrix projection;
}

struct Input
{
    float3 position : POSITION;
    float time : TIME; // Time (in seconds) particles has been active
    float3 velocity : VELOCITY;
    float rotationZ : ROTATIONZ;
    float3 rgb : RGB; // Red Green Blue
    float size : SIZE;
    int patterns : PATTERNS;
    int VFXpatterns : VFXPATTERNS;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV;
    float time : TIME;
    int patterns : PATTERNS;
    int VFXpatterns : VFXPATTERNS;
};

VS_OUT main(Input inval)
{
    VS_OUT retval;
    //float3 worldMid = mul(float4(meshMid, 1.f), world).xyz;
    //float3 look = cameraPosition.xyz - worldMid;
    ////look.y = 0.f;
    //look = normalize(look);
    
    
    //// TODO: Up vector needs to be calculated independantly
    //float3 up = float3(0.f, 1.f, 0.f);
    //float3 right = cross(up, look);
    
    //inval.position.z = 1.f;
    //retval.position.xyz += right + up;
    
    //retval.position = mul(float4(inval.position, 1.0f), world);
    //retval.position = mul(retval.position, view);
    //retval.position = mul(retval.position, projection);

    
    matrix worldView = mul(world, view);
    //float4 pos = float4(inval.position, 1.0f);
    //float4 WV = float4(worldView[3].xyz, 0.0f);
    
    retval.position = mul((float4(inval.position, 1.0f) + float4(worldView[3].xyz, 0.0f)), projection);
    retval.position /= retval.position.w;
    
    retval.rgb = float4(inval.rgb, 1.f);
    retval.uv = inval.rgb.xy;
    retval.time = inval.time;
    retval.patterns = inval.patterns;
    retval.VFXpatterns = inval.VFXpatterns;
    //retval.worldPosition = mul(retval.position, world);
    //retval.rotationZ = inval.rotationZ;
    //retval.size = inval.size;

    return retval;
}