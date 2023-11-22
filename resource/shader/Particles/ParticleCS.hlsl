#include "ParticleHeader.hlsli"

inline void SmokeMovement(in uint3 DTid, in uint3 blockID);
inline void ArchMovement(in uint3 DTid, in uint3 blockID);
inline void ExplosionMovement(in uint3 DTid, in uint3 blockID);
inline void FlamethrowerMovement(in uint3 DTid, in uint3 blockID);
inline void ImplosionMovement(in uint3 DTid, in uint3 blockID);
inline void RainMovement(in uint3 DTid, in uint3 blockID);
inline void LightningMovement(in uint3 DTid, in uint3 blockID);
inline void SpiralFieldMovement(in uint3 DTid, in uint3 blockID);
inline void ShockWaveMovement(in uint3 DTid, in uint3 blockID);

[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 blockID : SV_GroupID)
{    
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    
    if (index < meta[blockID.y].start || index > meta[blockID.y].end)
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
        // 8 = CIRCLE_FIELD
        if (meta[blockID.y].pattern == 8)
        {
            SpiralFieldMovement(DTid, blockID);
        }
        // 9 = PULSE
        if (meta[blockID.y].pattern == 9)
        {
            ShockWaveMovement(DTid, blockID);
        }
    }

}

inline void SmokeMovement(in uint3 DTid, in uint3 blockID)
{
    
    // -- Calculate the index and get the right particle to change -- //
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    int localIndex = (index - meta[blockID.y].start) % amount;
    
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
        float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + (float) ((float) localIndex / (float) amount), meta[blockID.y].startPosition.z);

        
        particle.position = startPosition;
        particle.time = 0.f;
    }
    if (particle.time >= (meta[blockID.y].life + meta[One_OneHundo].deltaTime))
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + (float) ((float) localIndex / (float) amount), meta[blockID.y].startPosition.z);

        particle.position = startPosition;
        particle.time = 0.f;
    }
            
    if (DTid.x % 2 == 1)
        particle.position.x = particle.position.x - (meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt;
    else
        particle.position.x = particle.position.x - ((meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt) * -1.0f;

    particle.position.y = particle.position.y + (meta[OneHundo_TwoFiveFive].deltaTime + meta[One_OneHundo].deltaTime) * dt;


    outputParticleData[index] = particle;
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
    // -- Calculate the index and get the right particle to change -- //
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    int localIndex = (index - meta[blockID.y].start) % amount;
    
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
            
    
    
    // EXLUSIVE FOR FLAME THROWER //
    // THEESE WEIRD VARIABLES ARE MEANT TO BE WEIRD, THEY HOLD VALUES
    float2 v0 = float2(meta[blockID.y].maxRange, meta[blockID.y].positionInfo.x);
    float2 v1 = float2(meta[blockID.y].positionInfo.y, meta[blockID.y].positionInfo.z);
    float2 v2 = float2(meta[blockID.y].morePositionInfo.x, meta[blockID.y].morePositionInfo.y);

    float2 legThree = v2 - v1;
    
    float2 dirVec = -normalize((legThree * ((float) localIndex / (float) amount)) + v1);
    
    
    float2 middlePoint = v1 + (legThree * 0.5f);
    float2 middleVector = middlePoint - v0;
    
    
    float2 calcPosition = float2(particle.position.x - meta[blockID.y].startPosition.x, particle.position.z - meta[blockID.y].startPosition.z);
    float2 v0ToParticle = calcPosition - v0;
    

    float v0ToParticle_len = length(v0ToParticle);
    float middleVector_len = length(middleVector);

    
    if (v0ToParticle_len < middleVector_len)
    {
        particle.position.x = particle.position.x + dirVec.x * (particle.velocity.x * 4.f) * dt * meta[OneHundo_TwoFiveFive].deltaTime;
        particle.position.y = particle.position.y; // +(((float) DTid.x - 127) / 128) * dt;
        particle.position.z = particle.position.z + dirVec.y * (particle.velocity.z * 4.f) * dt * meta[OneHundo_TwoFiveFive].deltaTime;
    }
    else
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);

        particle.position = startPosition;
        particle.time = 0.f;
        particle.size = 0.5f;
    }

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
    
    int localIndex = (index - meta[blockID.y].start) % amount;
    
    
    float posy = localIndex * 0.2f; // 51 / 255
    float idxFraction = localIndex / 255.f;
    float timeFraction = PI * (1 - (particle.time / meta[blockID.y].life));
    
    float alpha = pow(sin(2 * PI * idxFraction + timeFraction), 3); // Pi
    float beta = pow(sin(4 * 2.71828f * idxFraction + 4 * timeFraction), 3); // Eulers
    float gamma = pow(sin(6 * sqrt(5) * idxFraction + 9 * timeFraction), 3); // Root(5)
    
    particle.position.y = posy;
    particle.position.x = (2 * alpha + beta + 2 * gamma);
    particle.position.z = (alpha + 2 * beta - gamma);
    
    outputParticleData[index] = particle;
}

void SpiralFieldMovement(in uint3 DTid, in uint3 blockID)
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
    
    int localIndex = (index - meta[blockID.y].start) % amount;
    
    
    float indexValue = sqrt((10 + localIndex) / 265.f);
    float timeValue = (particle.time / meta[blockID.y].life);
    // "Uneven" circlings with a bit over 8 laps
    float piFraction = ((indexValue) * PI * 50.f) + timeValue * timeValue;
    // Expanding outwards
    float radius = meta[blockID.y].morePositionInfo.x * indexValue * sqrt(timeValue);
    
    particle.size = meta[blockID.y].size * /*sqrt*/(1 - timeValue);
    
    particle.position.x = cos(piFraction) * radius;
    particle.position.y = timeValue * indexValue;
    particle.position.z = sin(piFraction) * radius;
    
    outputParticleData[index] = particle;
}

void ShockWaveMovement(in uint3 DTid, in uint3 blockID)
{
        // -- Calculate the index and get the right particle to change -- //
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    int localIndex = (index - meta[blockID.y].start) % amount;
    
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

    
    if (travelledDistance >= meta[blockID.y].maxRange)
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y , meta[blockID.y].startPosition.z);
        
        
        particle.position = startPosition;
        particle.time = 0.f;
    }
    if (particle.time >= meta[blockID.y].life)
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);

        particle.position = startPosition;
        particle.time = 0.f;
    }
    
    particle.position.x = particle.position.x + meta[blockID.y].deltaTime * cos(particle.time) * dt; //cos(((float) localIndex / (float) amount) * PI) * particle.velocity.x * 30.f * dt;
    //particle.position.y = particle.position.y + 0.2f * sin(particle.time * 128.f);
        particle.position.z = particle.position.z + sin(particle.time) * dt; //sin(((float) localIndex / (float) amount) * PI) * particle.velocity.z * 30.f * dt;
    
    //if (localIndex % 2 == 0)
    //{
    //    particle.position.x = particle.position.x + cos(((float) localIndex / (float) amount) * PI) * particle.velocity.x * 30.f * dt;
    //    particle.position.y = particle.position.y + 0.2f * sin(particle.time * 128.f);
    //    particle.position.z = particle.position.z + sin(((float) localIndex / (float) amount) * PI) * particle.velocity.z * 30.f * dt;
    //}
    //else
    //{
    //    particle.position.x = particle.position.x - cos(((float) localIndex / (float) amount) * PI) * particle.velocity.x * 30.f * dt;
    //    particle.position.y = particle.position.y - 0.2f * sin(particle.time * 128.f);
    //    particle.position.z = particle.position.z - sin(((float) localIndex / (float) amount) * PI) * particle.velocity.z * 30.f * dt;
    //}
    
    
        outputParticleData[index] = particle;
    }