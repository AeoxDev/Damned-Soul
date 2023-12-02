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

struct metadata
{
    int start;
    int end;
    float life; // How much time (in seconds) a particle is allowed to live
    float maxRange; // How long particle is allowed to go 
    float3 startPosition;
    float size; // Size of particle
    float3 positionInfo;
    int pattern; // The movement pattern of the particle and decidor of size
    
    float deltaTime;
    // Slot 0 is reserved for delta time
    // Slot 1-99 is a random number between 0.0 and 1.0
    // Slot 100-255 is a random number between 1.0 and 5.0
    
    float2 morePositionInfo;
    bool reset;
};


cbuffer metadataBuffer : register(b2)
{
    metadata meta[256];
};

cbuffer metadataBuffer : register(b3)
{
    int metadataSlot;
    float timeAlive;
};


struct Input
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
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

    
    matrix worldView = mul(world, view);
    //float4 pos = float4(inval.position, 1.0f);
    //float4 WV = float4(worldView[3].xyz, 0.0f);
    
    retval.position = mul((float4(inval.position.xyz, 1.0f) + float4(worldView[3].xyz, 0.0f)), projection);
    retval.position /= retval.position.w;
    
    retval.rgb = float4(1.f, 1.f, 1.f, 1.f);
    retval.uv = inval.uv;
    retval.time = timeAlive;
    retval.patterns = meta[metadataSlot].pattern;
    // VFX pattern is stored in morePositionInfo.y to save space
    retval.VFXpatterns = meta[metadataSlot].morePositionInfo.y;
    //retval.worldPosition = mul(retval.position, world);
    //retval.rotationZ = inval.rotationZ;
    //retval.size = inval.size;

    return retval;
}