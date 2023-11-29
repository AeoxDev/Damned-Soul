#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER
#define NUM_THREADS 32

cbuffer ExtraData : register(b2)
{
    float3 meshMid;
    int start;
};

struct Input
{
    float3 position : POSITION;
    float time : TIME; // Time (in seconds) particles has been active
    float3 velocity : VELOCITY;
    float rotationZ : ROTATIONZ;
    float3 rgb : RGB; // Red Green Blue
    float size : SIZE;
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
    
    float2 morePositionInfo;
    bool reset;
};

cbuffer metadataBuffer : register(b0)
{
    metadata meta[256];
};

static const float PI = 3.14159265f;

RWStructuredBuffer<Input> outputParticleData : register(u0);
RWStructuredBuffer<Input> inputParticleData : register(u1);
#endif