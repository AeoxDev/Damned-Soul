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
    // 0 is reserved for delta time
    // 1 - 255 is a random number between 1.0 and 10.0
};

cbuffer metadataBuffer : register(b0)
{
    metadata meta[256];
};

static const float PI = 3.14159265f;


inline void SmokeMovement(in uint3 DTid, in uint3 blockID);
inline void ArchMovement(in uint3 DTid, in uint3 blockID);
inline void ExplosionMovement(in uint3 DTid, in uint3 blockID);
inline void FlamethrowerMovement(in uint3 DTid, in uint3 blockID);
inline void ImplosionMovement(in uint3 DTid, in uint3 blockID);
inline void RainMovement(in uint3 DTid, in uint3 blockID);

bool IsPointInTriangle(float2 spot, float2 v0, float2 v1, float2 v2);


RWStructuredBuffer<Input> inputParticleData : register(u0);
RWStructuredBuffer<Input> outputParticleData : register(u1);

[numthreads(NUM_THREADS, 1, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 blockID : SV_GroupID)
{

    

    
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
    }

}

inline void SmokeMovement(in uint3 DTid, in uint3 blockID)
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
    
    
    float travelledDistance = distance(particle.position, meta[blockID.y].startPosition);
    
    if (travelledDistance >= (meta[blockID.y].maxRange + meta[One_OneHundo].deltaTime))
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + ((float) DTid.x / NUM_THREADS), 1.0f);

        particle.position = startPosition;
        particle.time = 0.f;
    }
    if (particle.time >= (meta[blockID.y].life + meta[One_OneHundo].deltaTime))
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + ((float) DTid.x / NUM_THREADS), 1.0f);

        particle.position = startPosition;
        particle.time = 0.f;
    }
            
    if (DTid.x < 126)
        particle.position.x = particle.position.x + (meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt;
    else
        particle.position.x = particle.position.x + ((meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt) * -1.0f;

    particle.position.y = particle.position.y + (meta[OneHundo_TwoFiveFive].deltaTime + psuedoRand) * dt;


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

const static float e = 2.7182818f;

float sigmoid(float x)
{
    return pow(e, x) / (1 + pow(e, x));
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
    
    
    float2 v0 = float2(0.0, 0.0);
    float2 v1 = float2(10.0, 10.0);
    float2 v2 = float2(10.0, -10.0);
    
    if (IsPointInTriangle(particle.position.xy, v0, v1, v2))
    {
        particle.position.x = particle.position.x + particle.velocity.x * meta[OneHundo_TwoFiveFive].deltaTime * dt;
        particle.position.y = particle.position.y + ((float)DTid.x - 127) / 128 * dt;

    }
    else
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, 1.0f);

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

// Function to check if a point is inside a triangle
bool IsPointInTriangle(float2 spot, float2 v0, float2 v1, float2 v2)
{
    // Calculate barycentric coordinates
    float detT = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);
    float alpha = ((v1.y - v2.y) * (spot.x - v2.x) + (v2.x - v1.x) * (spot.y - v2.y)) / detT;
    float beta = ((v2.y - v0.y) * (spot.x - v2.x) + (v0.x - v2.x) * (spot.y - v2.y)) / detT;
    
    return alpha >= 0.0 && beta >= 0.0 && (alpha + beta) <= 1.0;
}
