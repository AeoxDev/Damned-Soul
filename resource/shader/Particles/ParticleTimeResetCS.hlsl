#include "ParticleHeader.hlsli"


[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 blockID : SV_GroupID)
{
    int index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    
    if (index < meta[blockID.y].start || index > meta[blockID.y].end)
        return;

    inputParticleData[index].position = float3(9999.f, 9999.f, 9999.f);
    inputParticleData[index].time = 0;
    inputParticleData[index].VFXpatterns = -1;
    inputParticleData[index].patterns = -1;
}