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

StructuredBuffer<Input> inputParticleData : register(t0);
RWStructuredBuffer<Input> outputParticleData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    Input particle = inputParticleData[DTid.x];
    //____________________________________________________________________
    float noiseAmp = 0.1f; //just noise amplitude, adjust value as needed
    float3 noise = noiseAmp * normalize(float3(sin(DTid.x), cos(DTid.x), sin(DTid.x + cos(DTid.x))));
    particle.position += noise;
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
}