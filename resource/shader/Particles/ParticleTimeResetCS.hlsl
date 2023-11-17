#include "ParticleHeader.hlsli"


[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 blockID : SV_GroupID)
{
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int index = blockID.x * NUM_THREADS + DTid.x;
    
    if (index > amount)
        return;
     
    inputParticleData[index].position = float3(9999.f, 9999.f, 9999.f);
    inputParticleData[index].time = 0;
    

    
    

}