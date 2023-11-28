#include "ParticleHeader.hlsli"

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

struct VS_OUT
{
    //float4 position : SV_POSITION;
    //float4 worldPosition : POSITION;
    //float4 rgb : RGB;
    //float rotationZ : ROTATIONZ;
    //float size : SIZE;
    float4 position : SV_POSITION;
    float4 rgb : RGB;
};

VS_OUT main(Input inval)
{
    VS_OUT retval;
    float3 worldMid = mul(float4(meshMid, 1.f), world).xyz;
    float3 look = cameraPosition.xyz - worldMid;
    //look.y = 0.f;
    look = normalize(look);
    
    
    // TODO: Up vector needs to be calculated independantly
    float3 up = float3(0.f, 1.f, 0.f);
    float3 right = cross(up, look);
    
    inval.position.z = 1.f;
    retval.position = mul(float4(inval.position, 1.0f), world);
    retval.position.xyz += right + up;
    
    retval.position = mul(retval.position, view);
    retval.position = mul(retval.position, projection);
	
    retval.rgb = float4(inval.rgb, 1.f);
    //retval.worldPosition = mul(retval.position, world);
    //retval.rotationZ = inval.rotationZ;
    //retval.size = inval.size;

    return retval;
}