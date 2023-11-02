#include "ParticleHeader.hlsli"

[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 blockID : SV_GroupID)
{
    // -- SAME FOR ALL FUNCTIONS -- //
    int index = DTid.x + blockID.y * NUM_THREADS;
    
    outputParticleData[index].position = float3(9999.f, 9999.f, 9999.f);
    outputParticleData[index].time = 0;
}