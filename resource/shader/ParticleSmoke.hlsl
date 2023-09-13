struct input
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
void main( uint3 DTid : SV_DispatchThreadID )
{
    input test = inputParticleData[DTid.x];
    test.position = test.position + float3(0.1f, 0.1f, 0.1f);
    outputParticleData[DTid.x] = test;
}