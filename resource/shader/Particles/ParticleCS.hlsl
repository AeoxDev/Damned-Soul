#include "ParticleHeader.hlsli"

StructuredBuffer<Input> readParticleData : register(t0);


inline void SmokeMovement(in uint3 DTid, in uint3 blockID, in int One_OneHundo, in int OneHundo_TwoFiveFive);
inline void FlamethrowerMovement(in uint3 DTid, in uint3 blockID, in int One_OneHundo, in int OneHundo_TwoFiveFive);
inline void RainMovement(in uint3 DTid, in uint3 blockID, in int One_OneHundo, in int OneHundo_TwoFiveFive);
inline void LightningMovement(in uint3 DTid, in uint3 blockID);

[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 blockID : SV_GroupID)
{
            
    // ----- USED TO ACCESS "RANDOM" SLOTS IN deltaTime ---- //
    float psuedoRand = sin(DTid.x * 71.01) * sin(DTid.x * 71.01);
    
    float holder = frac(sin(dot(DTid.x, float2(12.9898, 78.233))) * 43758.5453) * 100.f;
    
    int One_OneHundo = holder;
    if (One_OneHundo == 0)
        One_OneHundo = 1;
    
    int OneHundo_TwoFiveFive = One_OneHundo + 155;
    // -------------------------- //
    
    if (meta[blockID.y].life > 0)
    {
        // 0 = SMOKE
        if (meta[blockID.y].pattern == 0)
        {
            SmokeMovement(DTid, blockID, One_OneHundo, OneHundo_TwoFiveFive);
        }
        // 1 = ARCH
        if (meta[blockID.y].pattern == 1)
        {
            //ArchMovement(DTid, blockID);
        }
        // 2 = EXPLOSION
        if (meta[blockID.y].pattern == 2)
        {
            //ExplosionMovement(DTid, blockID);
        }
        // 3 = FLAMETHROWER
        if (meta[blockID.y].pattern == 3)
        {
            FlamethrowerMovement(DTid, blockID, One_OneHundo, OneHundo_TwoFiveFive);
        }
        // 4 = IMPLOSION
        if (meta[blockID.y].pattern == 4)
        {
            //ImplosionMovement(DTid, blockID);
        }
        // 5 = RAIN
        if (meta[blockID.y].pattern == 5)
        {
            //RainMovement(DTid, blockID);
        }
        // 6 = SINUS
        if (meta[blockID.y].pattern == 6)
        {
            //SinusMovement(DTid, blockID);
        }
        // 7 = LIGHTNING
        if (meta[blockID.y].pattern == 7)
        {
            LightningMovement(DTid, blockID);
        }
    }

}

inline void SmokeMovement(in uint3 DTid, in uint3 blockID, in int One_OneHundo, in int OneHundo_TwoFiveFive)
{
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int threadStart = ((amount * DTid.x) / NUM_THREADS) + meta[blockID.y].start;
    int threadEnd = ((amount + (DTid.x + 1)) / NUM_THREADS) + threadStart;
    
    float dt = meta[0].deltaTime;
    
    if (amount > NUM_THREADS)
    {
        for (int index = threadStart; index < threadEnd; index++)
        {
            if (index > meta[blockID.y].end)
                return;
        
            Input particle = readParticleData[index];
            particle.time = particle.time + dt;
            particle.size = meta[blockID.y].size;
    
    
            float travelledDistance = distance(particle.position, meta[blockID.y].startPosition);
    
            if (travelledDistance >= (meta[blockID.y].maxRange + meta[One_OneHundo].deltaTime))
            {
                float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + ((float) index / NUM_THREADS), meta[blockID.y].startPosition.z);

                particle.position = startPosition;
                particle.time = 0.f;
            }
            if (particle.time >= (meta[blockID.y].life + meta[One_OneHundo].deltaTime))
            {
                float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + ((float) index / NUM_THREADS), meta[blockID.y].startPosition.z);

                particle.position = startPosition;
                particle.time = 0.f;
            }
            
            if (DTid.x < 126)
                particle.position.x = particle.position.x - (meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt;
            else
                particle.position.x = particle.position.x - ((meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt) * -1.0f;

            particle.position.y = particle.position.y + (meta[OneHundo_TwoFiveFive].deltaTime + DTid.x / NUM_THREADS) * dt;

        
            outputParticleData[index] = particle;
        }
    }
    else
    {
        int index = meta[blockID.y].start + DTid.x;
        if (index > meta[blockID.y].end)
            return;
        
        Input particle = readParticleData[index];
        particle.time = particle.time + dt;
        particle.size = meta[blockID.y].size;

        
        float travelledDistance = distance(particle.position, meta[blockID.y].startPosition);
    
        if (travelledDistance >= (meta[blockID.y].maxRange + meta[One_OneHundo].deltaTime))
        {
            float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + ((float) index / NUM_THREADS), meta[blockID.y].startPosition.z);

            particle.position = startPosition;
            particle.time = 0.f;
        }
        if (particle.time >= (meta[blockID.y].life + meta[One_OneHundo].deltaTime))
        {
            float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + ((float) index / NUM_THREADS), meta[blockID.y].startPosition.z);

            particle.position = startPosition;
            particle.time = 0.f;
        }
            
        if (DTid.x < 126)
            particle.position.x = particle.position.x - (meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt;
        else
            particle.position.x = particle.position.x - ((meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt) * -1.0f;

        particle.position.y = particle.position.y + (meta[OneHundo_TwoFiveFive].deltaTime + DTid.x / NUM_THREADS) * dt;

        
        outputParticleData[index] = particle;
    }
}

void FlamethrowerMovement(in uint3 DTid, in uint3 blockID, in int One_OneHundo, in int OneHundo_TwoFiveFive)
{
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int threadStart = ((amount * DTid.x) / NUM_THREADS) + meta[blockID.y].start;
    int threadEnd = ((amount + (DTid.x + 1)) / NUM_THREADS) + threadStart;
    
    float dt = meta[0].deltaTime;
    
    if (amount > NUM_THREADS)
    {
        for (int index = threadStart; index < threadEnd; index++)
        {
            Input particle = readParticleData[index];
            particle.time = particle.time + dt;
            particle.size = meta[blockID.y].size;

            
            // EXLUSIVE FOR FLAME THROWER //
            // THEESE WEIRD VARIABLES ARE MEANT TO BE WEIRD, THEY HOLD VALUES
            float2 v0 = float2(meta[blockID.y].maxRange, meta[blockID.y].positionInfo.x);
            float2 v1 = float2(meta[blockID.y].positionInfo.y, meta[blockID.y].positionInfo.z);
            float2 v2 = float2(meta[blockID.y].morePositionInfo.x, meta[blockID.y].morePositionInfo.y);
            
            float inAm = (float) index / (float) amount;
            float2 legThree = (v2 - v1);
            legThree.y = meta[OneHundo_TwoFiveFive].deltaTime;
            float2 pointOnEnd = v1 + legThree * inAm;
            float2 dir = normalize(v0 - pointOnEnd);
            
            
            float2 middlePoint = (v2 - v1) / 2;
            float2 middleVector = v0 - middlePoint;
    
            float2 calcPos = float2(particle.position.x - meta[blockID.y].startPosition.x, particle.position.z - meta[blockID.y].startPosition.z);
            float2 v0ToParticle = v0 - calcPos;
    
            if (length(v0ToParticle) < length(middleVector))
            {
                //float angle = acos(abs(dot(legOne, legTwo)) / (length(legOne) * length(legTwo)));
                
                particle.position.x = particle.position.x + dir.x * particle.velocity.x * dt * meta[OneHundo_TwoFiveFive].deltaTime;
                particle.position.y = particle.position.y; // +(((float) DTid.x - 127) / 128) * dt;
                particle.position.z = particle.position.z + dir.y * particle.velocity.z * dt * meta[OneHundo_TwoFiveFive].deltaTime;

            }
            else
            {
                float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);
                
                particle.position = startPosition;
                particle.time = 0.f;
            }
            outputParticleData[index] = particle;
        }
    }
    else
    {
        int index = meta[blockID.y].start + DTid.x;
        if (index > meta[blockID.y].end)
            return;
        
        Input particle = readParticleData[index];
        particle.time = particle.time + dt;
        particle.size = meta[blockID.y].size;
     
        // EXLUSIVE FOR FLAME THROWER //
        // THEESE WEIRD VARIABLES ARE MEANT TO BE WEIRD, THEY HOLD VALUES
        float2 v0 = float2(meta[blockID.y].maxRange, meta[blockID.y].positionInfo.x);
        float2 v1 = float2(meta[blockID.y].positionInfo.y, meta[blockID.y].positionInfo.z);
        float2 v2 = float2(meta[blockID.y].morePositionInfo.x, meta[blockID.y].morePositionInfo.y);
    
        float2 legOne = v1 - v0;
        float2 legTwo = v2 - v0;
    
        float2 middlePoint = (v2 - v1) / 2;
        float2 middleVector = middlePoint - v0;
    
        float2 dirVec = normalize(middlePoint - v0);
    
        float2 v0ToParticle = particle.position.xz - v0;
    
        float alpha = acos(dot(legOne, middleVector) / (length(legOne) * length(middleVector))) * 0.25f;
        float beta = ((alpha * 2 * ((float) index / NUM_THREADS))) - alpha + PI * 0.5f;
    
        if (length(v0ToParticle) < length(middleVector))
        {
            float angle = atan2(dot(legOne, v0ToParticle), dot(legOne, legTwo));
            
            particle.position.x = particle.position.x + cos(angle) * particle.velocity.x * dt * meta[OneHundo_TwoFiveFive].deltaTime;
            particle.position.y = particle.position.y; // +(((float) DTid.x - 127) / 128) * dt;
            particle.position.z = particle.position.z + sin(angle) * particle.velocity.z * dt * meta[OneHundo_TwoFiveFive].deltaTime;

        }
        else
        {
            float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);

            particle.position = startPosition;
            particle.time = 0.f;
        }

        outputParticleData[index] = particle;
    }

    /*
    Maybe doesnt work? 
    
                Input particle = readParticleData[index];
            particle.time = particle.time + dt;
            particle.size = meta[blockID.y].size;

            
            // EXLUSIVE FOR FLAME THROWER //
            // THEESE WEIRD VARIABLES ARE MEANT TO BE WEIRD, THEY HOLD VALUES
            float2 v0 = float2(meta[blockID.y].maxRange, meta[blockID.y].positionInfo.x);
            float2 v1 = float2(meta[blockID.y].positionInfo.y, meta[blockID.y].positionInfo.z);
            float2 v2 = float2(meta[blockID.y].morePositionInfo.x, meta[blockID.y].morePositionInfo.y);
    
            float2 legOne = v1 - v0;
            float2 legTwo = v2 - v0;
    
            float normal = normalize(cross(float3(legOne, 0.f), float3(legTwo, 0.f)));

            float2 v0ToParticle = particle.position.xz - v0;

            
            float2 middlePoint = (v2 - v1) / 2;
            float2 middleVector = middlePoint - v0;
    
            float2 dirVec = normalize(middlePoint - v0);
            
            
    
            float alpha = acos(dot(legOne, middleVector) / (length(legOne) * length(middleVector))) * 0.25f;
            float beta = ((alpha * 2 * ((float) index / NUM_THREADS))) - alpha + PI * 0.5f;
            
            beta = acos(abs(dot(legOne, legTwo)) / (length(legOne) * length(legTwo)));

            float crossDirTwo = v0ToParticle.x * legTwo.y - v0ToParticle.y * legTwo.x;
            
            // We use Y but we are actully calculating for the X and Z coordinates
            float magOne = (v0.x - particle.position.x) * (v1.y - v0.y) - (v0.y - (particle.position.z - meta[blockID.y].startPosition.z)) * (v1.x - v0.x);
            float magTwo = (v0.x - particle.position.x) * (v2.y - v0.y) - (v0.y - (particle.position.z - meta[blockID.y].startPosition.z)) * (v2.x - v0.x);
            
            float v0ToPartLEN = length(v0ToParticle);
            float middleLEN = length(middleVector);
            
            // Check if the particle is on one side of legOne
            if ((magOne >= 0 && magTwo <= 0) && v0ToPartLEN < middleLEN)
            {
                particle.position.x = particle.position.x + cos(beta) * particle.velocity.x * dt * meta[OneHundo_TwoFiveFive].deltaTime;
                particle.position.y = particle.position.y; // +(((float) DTid.x - 127) / 128) * dt;
                particle.position.z = particle.position.z + sin(beta) * particle.velocity.z * dt * meta[OneHundo_TwoFiveFive].deltaTime;

            }
            else
            {
                float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);
                
                particle.position = startPosition;
                particle.time = 0.f;
            }
            outputParticleData[index] = particle;
    */
    
}

void ImplosionMovement(in uint3 DTid, in uint3 blockID, int One_OneHundo, int OneHundo_TwoFiveFive)
{
    int index = DTid.x + blockID.y * NUM_THREADS;
    
    Input particle = readParticleData[DTid.x];
    //____________________________________________________________________
    float3 direction = normalize(float3(0.f, 0.f, 0.f) - particle.position); // FIX THIS FELIX PLEASE, WE NEED TO ADD IMPLPSION POINT
    float implosionSpeed = 8.0f; //adjust as you see fit
    particle.position += direction * implosionSpeed;
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
}

void RainMovement(in uint3 DTid, in uint3 blockID)
{
    int index = DTid.x + blockID.y * NUM_THREADS;
    
    Input particle = readParticleData[DTid.x];
    //____________________________________________________________________
    float speedOfRain = 4.0f; //adjust as you see fit
    float scatterAmount = 4.0f; // adjust as you see fit
    float scatterX = (float(DTid.x % 100) / 100.0f - 0.5f) * scatterAmount; // adjust as you see fit
    float scatterZ = (float(DTid.x % 57) / 57.0f - 0.5f) * scatterAmount; // adjust as you see fit

    particle.position.x += scatterX;
    particle.position.z += scatterZ;
    particle.position.y -= speedOfRain;
    
    
    if (particle.position.y < 0.0f)
    {
        particle.position = meta[index].startPosition;
    }
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
}

void LightningMovement(in uint3 DTid, in uint3 blockID)
{
// -- SAME FOR ALL FUNCTIONS -- //
    uint index = (DTid.x + blockID.y * NUM_THREADS);
    Input particle = readParticleData[index];
    
    float dt = meta[0].deltaTime;
    particle.time = particle.time + dt;
    particle.size = meta[blockID.y].size;
    
    
    float posy = (index % 256) * 0.2f; // 51 / 255
    float idxFraction = (index % 256) / 255.f;
    float timeFraction = PI * (1 - (particle.time / meta[blockID.y].life));
    
    float alpha = pow(sin(2 * PI * idxFraction + timeFraction), 3); // Pi
    float beta = pow(sin(4 * 2.71828f * idxFraction + 4 * timeFraction), 3); // Eulers
    float gamma = pow(sin(6 * sqrt(5) * idxFraction + 9 * timeFraction), 3); // Root(5)
    
    particle.position.y = posy;
    particle.position.x = (2 * alpha + beta + 2 * gamma);
    particle.position.z = (alpha + 2 * beta - gamma);
    
    outputParticleData[index] = particle;
}

// Function to check if a point is inside a triangle
bool IsPointInTriangle(float2 A, float2 B, float2 C, float2 P)
{  
    bool retVal = false;
    
    float v0 = C - A;
    float v1 = B - A;
    float v2 = P - A;
    
    float dot00 = dot(v0, v0);
    float dot01 = dot(v0, v1);
    float dot02 = dot(v0, v2);
    float dot11 = dot(v1, v1);
    float dot12 = dot(v1, v2);
    
    float denom = dot00 * dot11 - dot01 * dot01;
    float u = (dot11 * dot02 - dot01 * dot12) / denom;
    float v = (dot00 * dot12 - dot01 * dot02) / denom;
    float w = 1.0f - u - v;
    
    if (u >= 0 && v >= 0 && w >= 0 && u+v+w == 1)
        retVal = true;
    
    return retVal;
}
