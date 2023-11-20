#include "ParticleHeader.hlsli"

//StructuredBuffer<Input> readParticleData : register(t0);

inline void SmokeMovement(in uint3 DTid, in uint3 blockID);
inline void ArchMovement(in uint3 DTid, in uint3 blockID);
inline void ExplosionMovement(in uint3 DTid, in uint3 blockID);
inline void FlamethrowerMovement(in uint3 DTid, in uint3 blockID);
inline void ImplosionMovement(in uint3 DTid, in uint3 blockID);
inline void RainMovement(in uint3 DTid, in uint3 blockID);
inline void LightningMovement(in uint3 DTid, in uint3 blockID);

bool IsPointInTriangle(float2 particleVector, float2 triangleVector);

[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 blockID : SV_GroupID)
{    
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int index = blockID.x * NUM_THREADS + DTid.x;
    
    if (index > amount)
        return;
    
    
    if (meta[blockID.y].life > 0)
    {
        // 0 = SMOKE
        if (meta[blockID.y].pattern == 0)
        {
            SmokeMovement(DTid, blockID);
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
            FlamethrowerMovement(DTid, blockID);
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

inline void SmokeMovement(in uint3 DTid, in uint3 blockID)
{
    
    // -- Calculate the index and get the right particle to change -- //
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    
    Input particle = inputParticleData[index];
    // -------------------------------------------------------------- // 

    
    // --- Set the standard stuff --- //
    float dt = meta[0].deltaTime;
    particle.time = particle.time + dt;
    particle.size = meta[blockID.y].size;
    // ------------------------------ //
    
    
    // ---- Get a "randomized" value to access deltaTime ---- //    
    float psuedoRand = sin(index * 71.01) * sin(index * 71.01);
    
    float holder = frac(sin(dot(index, float2(12.9898, 78.233))) * 43758.5453) * 100.f;
    
    int One_OneHundo = holder;
    if (One_OneHundo == 0)
        One_OneHundo = 1;
    
    int OneHundo_TwoFiveFive = One_OneHundo + 155;
    // ------------------------------------------------------ //
            
            

    float travelledDistance = distance(particle.position, meta[blockID.y].startPosition);
    
    if (travelledDistance >= (meta[blockID.y].maxRange + meta[One_OneHundo].deltaTime))
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + (float) ((float) index / (float) amount), meta[blockID.y].startPosition.z);

        
        particle.position = startPosition;
        particle.time = 0.f;
    }
    if (particle.time >= (meta[blockID.y].life + meta[One_OneHundo].deltaTime))
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + (float) ((float) index / (float) amount), meta[blockID.y].startPosition.z);

        particle.position = startPosition;
        particle.time = 0.f;
    }
            
    if (DTid.x % 2 == 1)
        particle.position.x = particle.position.x - (meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt;
    else
        particle.position.x = particle.position.x - ((meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt) * -1.0f;

    particle.position.y = particle.position.y + (meta[OneHundo_TwoFiveFive].deltaTime + meta[One_OneHundo].deltaTime) * dt;


    outputParticleData[index] = particle;

    
    
    
    
    
    //// -- SAME FOR ALL FUNCTIONS -- //
    //int index = DTid.x + blockID.y * NUM_THREADS;
    
    //// - NEW - //
    //int amount = meta[blockID.y].end - meta[blockID.y].start;
    //index = meta[blockID.y].start + DTid.x;
    //if (index > meta[blockID.y].end)
    //    return;
    //// - //
    
    //Input particle = outputParticleData[index];
    
    //float dt = meta[0].deltaTime;
    //particle.time = particle.time + dt;
    //particle.size = meta[blockID.y].size;
    
    //float psuedoRand = sin(DTid.x * 71.01) * sin(DTid.x * 71.01);
    
   
    //float holder = frac(sin(dot(DTid.x, float2(12.9898, 78.233))) * 43758.5453) * 100.f;
    
    //int One_OneHundo = holder;
    //if (One_OneHundo == 0)
    //    One_OneHundo = 1;
    
    //int OneHundo_TwoFiveFive = One_OneHundo + 155;
    //// -------------------------- //
    
    
    //float travelledDistance = distance(particle.position, meta[blockID.y].startPosition);
    
    //if (travelledDistance >= (meta[blockID.y].maxRange + meta[One_OneHundo].deltaTime))
    //{
    //    float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + ((float) DTid.x / NUM_THREADS), meta[blockID.y].startPosition.z);

    //    particle.position = startPosition;
    //    particle.time = 0.f;
    //}
    //if (particle.time >= (meta[blockID.y].life + meta[One_OneHundo].deltaTime))
    //{
    //    float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + ((float) DTid.x / NUM_THREADS), meta[blockID.y].startPosition.z);

    //    particle.position = startPosition;
    //    particle.time = 0.f;
    //}
            
    //if (DTid.x < 126)
    //    particle.position.x = particle.position.x - (meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt;
    //else
    //    particle.position.x = particle.position.x - ((meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt) * -1.0f;

    //particle.position.y = particle.position.y + (meta[OneHundo_TwoFiveFive].deltaTime + psuedoRand) * dt;


    //outputParticleData[index] = particle;
}

void ArchMovement(in uint3 DTid, in uint3 blockID)
{
    int index = DTid.x + blockID.y * NUM_THREADS;
    
    Input particle = inputParticleData[DTid.x];
    //____________________________________________________________________
    float time = DTid.x; // * (end - start) / float(end); FIX THIS FELIX PLEASE,
    float coefficient = 0.5; //for parabolic path, change as you see fit
    particle.position.y = meta[index].startPosition.y + coefficient * time * time;
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
}

void ExplosionMovement(in uint3 DTid, in uint3 blockID)
{
    int index = DTid.x + blockID.y * NUM_THREADS;
    
    Input particle = inputParticleData[DTid.x];
    //____________________________________________________________________
    float3 directionRandom = normalize(float3((DTid.x % 100) / 100.0f - 0.5f, (DTid.x % 57) / 57.0f - 0.5f, (DTid.x % 83) / 83.0f - 0.5f));
    float explosionSpeed = 8.0f; //adjust as you see fit
    float distance = length(particle.position - meta[index].startPosition);
    
    particle.position += directionRandom * explosionSpeed * distance;
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
}

void FlamethrowerMovement(in uint3 DTid, in uint3 blockID)
{
    // -- SAME FOR ALL FUNCTIONS -- //
    int index = DTid.x + blockID.y * NUM_THREADS;
    Input particle = inputParticleData[index];
    
    float dt = meta[0].deltaTime;
    particle.time = particle.time + dt;
    particle.size = meta[blockID.y].size;
    
    float psuedoRand = sin(DTid.x * 71.01) * sin(DTid.x * 71.01);
    
   
    float holder = frac(sin(dot(DTid.x, float2(12.9898, 78.233))) * 43758.5453) * 100.f;
    
    int One_OneHundo = holder;
    if (One_OneHundo == 0)
        One_OneHundo = 1;
    
    int OneHundo_TwoFiveFive = One_OneHundo + 155;
    // -------------------------- //
    
    
    // EXLUSIVE FOR FLAME THROWER //
    // THEESE WEIRD VARIABLES ARE MEANT TO BE WEIRD, THEY HOLD VALUES
    float2 v0 = float2(meta[blockID.y].positionInfo.y, meta[blockID.y].positionInfo.z);
    float2 v1 = float2(meta[blockID.y].morePositionInfo.x, meta[blockID.y].morePositionInfo.y);
    float2 v2 = float2(meta[blockID.y].morePositionInfo.z, meta[blockID.y].morePositionInfo.w);
    
    float2 legOne = v1 - v0;
    float2 legTwo = v2 - v0;
    
    float2 middlePoint = (v2 - v1) / 2;
    float2 middleVector = middlePoint - v0;
    
    
    
    
    float2 dirVec = normalize(middlePoint - v0);
    
    
    float2 v0ToParticle = particle.position.xz - v0;
    
    
    float alpha = acos(dot(legOne, middleVector) / (length(legOne) * length(middleVector))) * 0.25f;
    float beta = ((alpha * 2 * ((float) DTid.x / NUM_THREADS))) - alpha + PI * 0.5f;

    
    
    if (length(v0ToParticle) < length(middleVector))
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

    
    
    
    //if (IsPointInTriangle(particle.position.xy, v0, v1, v2))
    //{
    //    float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, 1.0f);

    //    particle.position = startPosition;
    //    particle.time = 0.f;
    //}
    //else if (particle.time >= (meta[blockID.y].life + meta[One_OneHundo].deltaTime))
    //{
    //    float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, 1.0f);

    //    particle.position = startPosition;
    //    particle.time = 0.f;
    //}
    
    //particle.position.x = particle.position.x + (particle.time * particle.velocity.x * meta[OneHundo_TwoFiveFive].deltaTime) * dt;
    //if (DTid.x < 85)
    //    particle.position.y = particle.position.y + (particle.time * particle.velocity.y) * dt;
    //else if (DTid.x < 170)
    //    particle.position.y = particle.position.y + particle.velocity.y * dt;
    //else
    //    particle.position.y = particle.position.y + ((particle.time * particle.velocity.y) * dt) * -1.f;
    
   

    outputParticleData[index] = particle;
}

void ImplosionMovement(in uint3 DTid, in uint3 blockID)
{
    int index = DTid.x + blockID.y * NUM_THREADS;
    
    Input particle = inputParticleData[DTid.x];
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
    
    Input particle = inputParticleData[DTid.x];
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
    Input particle = inputParticleData[index];
    
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
bool IsPointInTriangle(float2 particleVector, float2 triangleVector)
{
    //float degree = dot(particleVector, triangleVector) / length(triangleVector);
    float scalar = dot(particleVector, triangleVector) / (length(particleVector) * length(triangleVector));

    return true;
}
