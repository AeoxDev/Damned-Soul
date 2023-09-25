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
    float3 centerPosition; // implosion point
};

StructuredBuffer<Input> inputParticleData : register(t0);
RWStructuredBuffer<Input> outputParticleData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    Input particle = inputParticleData[DTid.x];
    //____________________________________________________________________
    float3 direction = normalize(centerPosition - particle.position);
    float implosionSpeed = 8.0f; //adjust as you see fit
    particle.position += direction * implosionSpeed;
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
}