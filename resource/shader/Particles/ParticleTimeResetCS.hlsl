#include "ParticleHeader.hlsli"

[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 blockID : SV_GroupID)
{
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int threadStart = ((amount * DTid.x) / NUM_THREADS) + meta[blockID.y].start;
    int threadEnd = ((amount + (DTid.x + 1)) / NUM_THREADS) + meta[blockID.y].start;
    
    float dt = meta[0].deltaTime;
    
    if (amount > NUM_THREADS)
    {
        for (int index = threadStart; index < threadEnd; index++)
        {
            if (index > meta[blockID.y].end)
                return;
            
            outputParticleData[index].position = float3(9999.f, 9999.f, 9999.f);
            outputParticleData[index].time = 0;
        }
    }
    else
    {
        int index = meta[blockID.y].start + DTid.x;
        if (index > meta[blockID.y].end)
            return;
  
        outputParticleData[index].position = float3(9999.f, 9999.f, 9999.f);
        outputParticleData[index].time = 0;
    }
    
    

}