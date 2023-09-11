struct input
{
    float3 position;
    float padding1;
    float3 velocity;
    float padding2;
    float3 rgb;
    float padding3;

};

cbuffer metadata : register (b0)
{
    int start;
    int end;
    float life;
    float maxRange;
    
    float3 startPosition;
};



StructuredBuffer<input> inputParticleData : register( t0 ); 
RWStructuredBuffer<input> outputParticleData : register( u0 );

[numthreads(100, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    outputParticleData[DTid.x].position = inputParticleData[DTid.x].position + float3(0.1f, 0.1f, 0.1f);
}