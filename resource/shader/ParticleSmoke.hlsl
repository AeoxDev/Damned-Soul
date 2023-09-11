struct input
{
    float3 position;
    float padding1;
    float3 velocity;
    float padding2;
    float3 rgb;
    float padding3;

};


StructuredBuffer<input> inputParticleData : register( t0 ); 
RWStructuredBuffer<input> outputParticleData : register( u0 );

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    
}