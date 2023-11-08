#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER
#define NUM_THREADS 256

struct Input
{
    float3 position;
    float time; // Time (in seconds) particles has been active
    float3 velocity;
    float rotationZ;
    float3 rgb; // Red Green Blue
    float size;
    float4 patterns;
};

struct metadata
{
    float life; // How much time (in seconds) a particle is allowed to live
    float maxRange; // How long particle is allowed to go 
    float size; // Size of particle
    int pattern; // The movement pattern of the particle and decidor of size
    
    float3 startPosition;
    float deltaTime;
    // Slot 0 is reserved for delta time
    // Slot 1-99 is a random number between 0.0 and 1.0
    // Slot 100-255 is a random number between 1.0 and 5.0
    
    float rotationY;
    float3 positionInfo;
    float4 morePositionInfo;
};

cbuffer metadataBuffer : register(b0)
{
    metadata meta[256];
};

static const float PI = 3.14159265f;

RWStructuredBuffer<Input> inputParticleData : register(u0);
RWStructuredBuffer<Input> outputParticleData : register(u1);
#endif