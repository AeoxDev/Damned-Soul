struct Input
{
    float3 position;
    float padding1;
    float3 velocity;
    float padding2;
    float3 rgb;
    float padding3;
    
};

cbuffer metadata : register(b0)
{
    int start;
    int end;
    float life;
    float maxRange;
    float3 startPosition;
};

StructuredBuffer<input> inputParticleData : register(t0);
RWStructuredBuffer<input> outputParticleData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    Input particle = inputParticleData[DTid.x];
    //____________________________________________________________________
    float time = DTid.x * (end - start) / float(end);
    float coefficient = 0.5; //for parabolic path, change as you see fit
    particle.position.y = startPosition.y + coefficient * time * time;
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
}