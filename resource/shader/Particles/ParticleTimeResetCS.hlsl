#include "Headers/ParticleHeader.hlsli"


[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 blockID : SV_GroupID)
{
    int index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    
    if (index < meta[blockID.y].start || index > meta[blockID.y].end || meta[blockID.y].reset == 0)
        return;

    inputParticleData[index].position = float3(99999.f, 99999.f, 99999.f);
    inputParticleData[index].time = 0;
    inputParticleData[index].VFXpatterns = -1;
    inputParticleData[index].patterns = -1;   
    outputParticleData[index].position = float3(99999.f, 99999.f, 99999.f);
    outputParticleData[index].time = 0;
    outputParticleData[index].VFXpatterns = -1;
    outputParticleData[index].patterns = -1;
}