#define NUM_THREADS 256

struct Input
{
    float3 position;
    float time; // Time (in seconds) particles has been active
    float3 velocity;
    float rotationZ;
    float3 rgb; // Red Green Blue
    float size;
};

struct metadata
{
    float life; // How much time (in seconds) a particle is allowed to live
    float maxRange; // How long particle is allowed to go 
    float size; // Size of particle
    int pattern; // The movement pattern of the particle and decidor of size
    
    float3 startPosition;
    float deltaTime;
};

cbuffer metadataBuffer : register(b0)
{
    metadata meta[256];
};

void SmokeMovement(in uint3 DTid, in uint3 blockID);
void ArchMovement(in uint3 DTid, in uint3 blockID);
void ExplosionMovement(in uint3 DTid, in uint3 blockID);
void FlamethrowerMovement(in uint3 DTid, in uint3 blockID);
void ImplosionMovement(in uint3 DTid, in uint3 blockID);
void RainMovement(in uint3 DTid, in uint3 blockID);
void SinusMovement(in uint3 DTid, in uint3 blockID);

StructuredBuffer<Input> inputParticleData : register(t0);
RWStructuredBuffer<Input> outputParticleData : register(u0);

[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID, uint3 blockID : SV_GroupID)
{
    int index = (DTid.x + blockID.y * NUM_THREADS) - 1; // -1 because we start on 0
    
    outputParticleData[index].position.y = inputParticleData[index].position.y + 1.0f;
    outputParticleData[index].time = inputParticleData[index].time + meta[blockID.y].deltaTime;
    outputParticleData[index].size = meta[blockID.y].size;
    
    float psudeoRand = sin((meta[blockID.y].deltaTime * 34579.41337f) * (cos(DTid.x * 35317.9870f)));
    float distance = dot(outputParticleData[index].position, meta[blockID.y].startPosition);
    float3 startPosition = float3(psudeoRand, psudeoRand, psudeoRand);
    
    if (distance >= meta[blockID.y].maxRange)
    {
        outputParticleData[index].position = startPosition;
        outputParticleData[index].time = 0.f;
    }
    if (outputParticleData[index].time >= meta[blockID.y].life)
    {
        outputParticleData[index].position = startPosition;
        outputParticleData[index].time = 0.f;
    }

    
   
    //if (particle.position.y >= 10.f)
    //{
    //    particle.position.y = 0.f;
    //    particle.time = 0.f;
    //}
    //else if (particle.time >= meta[blockID.y].life)
    //{
    //    particle.position.y = 0.f;
    //    particle.time = 0.f;
    //}
    
    //if (meta[blockID.y].life > 0)
    //{
    //    // 0 = SMOKE
    //    if (meta[blockID.y].pattern == 0)
    //        SmokeMovement(DTid, blockID);
    //    // 1 = ARCH
    //    else if (meta[blockID.y].pattern == 1)
    //        ArchMovement(DTid, blockID);
    //    // 2 = EXPLOSION
    //    else if (meta[blockID.y].pattern == 2)
    //        ExplosionMovement(DTid, blockID);
    //    // 3 = FLAMETHROWER
    //    else if (meta[blockID.y].pattern == 3)
    //        FlamethrowerMovement(DTid, blockID);
    //    // 4 = IMPLOSION
    //    else if (meta[blockID.y].pattern == 4)
    //        ImplosionMovement(DTid, blockID);
    //    // 5 = RAIN
    //    else if (meta[blockID.y].pattern == 5)
    //        RainMovement(DTid, blockID);
    //    // 6 = SINUS
    //    else if (meta[blockID.y].pattern == 6)
    //        SinusMovement(DTid, blockID);
    //}

}

void SmokeMovement(in uint3 DTid, in uint3 blockID)
{
    int index = DTid.x + blockID.y * NUM_THREADS;
   
    
    Input particle = inputParticleData[index];
    
    particle.time += meta[blockID.y].deltaTime;
    //____________________________________________________________________
    float noiseAmp = 0.1f; //just noise amplitude, adjust value as needed
    float3 noise = noiseAmp * normalize(float3(sin(DTid.x), cos(DTid.x), sin(DTid.x + cos(DTid.x))));
    particle.position += noise;
    //____________________________________________________________________
    //test.position = test.position;
    
    float psudeoRand = sin((meta[blockID.y].deltaTime * 34579.41337f) * (cos(DTid.x * 35317.9870f)));
    
    float distance = dot(particle.position, meta[blockID.y].startPosition);
    
    float3 startPosition = float3(psudeoRand, psudeoRand, psudeoRand);
    
    if (distance >= meta[blockID.y].maxRange)
        particle.position = startPosition;
    if (particle.time >= meta[blockID.y].life)
        particle.position = startPosition;
    
    
    
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
    int index = DTid.x + blockID.y * NUM_THREADS;
    
    Input particle = inputParticleData[DTid.x];
    //____________________________________________________________________
    float speedOfProjectile = 8.0f; // adjust as you see fit 
    
    //this is in radians, basically the spread of the cone
    float angleOfCone = 45.0f * (3.14159f / 180.0f); // Convert degrees to radians. Feel free to change angle (first number) as you see fit
    
    float radianAngle = 0; // FIX THIS FELIX PLEASE, ((float) DTid.x / float(end) - 0.5f) * angleOfCone;
    float3 direction = normalize(float3(cos(radianAngle), -1.0f, sin(radianAngle)));
    
    particle.position += direction * speedOfProjectile;

    // Reset particles that have moved out of range to the starting position
    if (particle.position.y < 0.0f)
    {
        particle.position = meta[index].startPosition;
    }
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
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

void SinusMovement(in uint3 DTid, in uint3 blockID)
{
    int index = DTid.x + blockID.y * NUM_THREADS;
    
    Input particle = inputParticleData[DTid.x];
    //____________________________________________________________________
    float time = DTid.x; //* (end - start) / float(end); FIX THIS FELIX PLEASE,
    float amplitude = 5.f; // Change value as you see fit
    float frequency = 1.f; // Change value as you see fit
    particle.position.y = meta[index].startPosition.y + sin(frequency * time) * amplitude;
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
}

