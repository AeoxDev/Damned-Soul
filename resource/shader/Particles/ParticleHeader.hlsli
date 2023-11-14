#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER
#define NUM_THREADS 32

struct Input
{
    float3 position;
    float time; // Time (in seconds) particles has been active
    float3 velocity;
    float rotationZ;
    float3 rgb; // Red Green Blue
    float size;
    //float padding;
};

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
    
    float rotationY;
    float2 morePositionInfo;
};

cbuffer metadataBuffer : register(b0)
{
    metadata meta[256];
};

static const float PI = 3.14159265f;

RWStructuredBuffer<Input> outputParticleData : register(u0);
#endif