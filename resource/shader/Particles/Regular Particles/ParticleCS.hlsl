#include "../Headers/ParticleHeader.hlsli"

inline void SmokeMovement(in uint3 DTid, in uint3 blockID);
inline void FlamethrowerMovement(in uint3 DTid, in uint3 blockID);
inline void LightningMovement(in uint3 DTid, in uint3 blockID);
inline void SpiralFieldMovement(in uint3 DTid, in uint3 blockID);
inline void FireMovement(in uint3 DTid, in uint3 blockID);
inline void ShockWaveMovement(in uint3 DTid, in uint3 blockID);
inline void HotPotMovement(in uint3 DTid, in uint3 blockID);
inline void SparkMovement(in uint3 DTid, in uint3 blockID);
inline void NoMovement(in uint3 DTid, in uint3 blockID);


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
        if (meta[blockID.y].pattern == 3 || meta[blockID.y].pattern == 11)
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
        // 9 = FIRE
        if (meta[blockID.y].pattern == 9)
        {
            FireMovement(DTid, blockID);
        }
        // 10 = PULSE
        if (meta[blockID.y].pattern == 10)
        {
            ShockWaveMovement(DTid, blockID);
        }
        // 11 SHARES SPOT WITH 3
   
        // 12 = BOILING
        if (meta[blockID.y].pattern == 12)
        {
            HotPotMovement(DTid, blockID);
        }
        // 13 = SPARK
        if (meta[blockID.y].pattern == 13)
        {
            SparkMovement(DTid, blockID);
        }
        // 14 = NO MOVEMENT
        if (meta[blockID.y].pattern == 14)
        {
            NoMovement(DTid, blockID);
        }
        
    }

}

void SmokeMovement(in uint3 DTid, in uint3 blockID)
{
    
    // -- Calculate the index and get the right particle to change -- //
    uint amount = meta[blockID.y].end - meta[blockID.y].start;
    uint index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    uint localIndex = (index - meta[blockID.y].start) % amount;
    
    Input particle = inputParticleData[index];
    // -------------------------------------------------------------- // 

    
    // --- Set the standard stuff --- //
    float dt = meta[0].deltaTime;
    particle.time = particle.time + dt;
    particle.size = meta[blockID.y].size;
    particle.patterns = meta[blockID.y].pattern;
        //VFX Pattern is stored in morePositionInfo.y to save space
    particle.VFXpatterns = (int) meta[blockID.y].morePositionInfo.y;
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
       //Z-edited
        //float oddEvenFactor = ((index % 2) - 0.5f) * 2; //gives values 0 or 1 based on particle 
        //startPosition.z= startPosition.z + index + oddEvenFactor * (4 * meta[index].deltaTime);
        ////
        particle.position = startPosition;
        particle.time = 0.f;
    }
    if (particle.time >= (meta[blockID.y].life + meta[One_OneHundo].deltaTime))
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x + meta[OneHundo_TwoFiveFive].deltaTime, meta[blockID.y].startPosition.y + (float) ((float) localIndex / (float) amount), meta[blockID.y].startPosition.z);
        //Z-edited
        //float oddEvenFactor = ((index % 2) - 0.5f) * 2; //gives values 0 or 1 based on particle 
        //startPosition.z = startPosition.z + index + oddEvenFactor * (4 * meta[index].deltaTime);
        ////
        particle.position = startPosition;
        particle.time = 0.f;
    }
            
    if (DTid.x % 2 == 1)
        particle.position.x = particle.position.x - (meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt;
    else
        particle.position.x = particle.position.x - ((meta[OneHundo_TwoFiveFive].deltaTime * (cos(particle.time * meta[OneHundo_TwoFiveFive].deltaTime))) * dt) * -1.0f;

    particle.position.y = particle.position.y + (meta[OneHundo_TwoFiveFive].deltaTime + meta[One_OneHundo].deltaTime) * dt;
    
    particle.rgb.r = 0.60f;
    particle.rgb.g = 0.60f;
    particle.rgb.b = 0.60f;

    outputParticleData[index] = particle;
}

void FlamethrowerMovement(in uint3 DTid, in uint3 blockID)
{
    // -- Calculate the index and get the right particle to change -- //
    uint amount = meta[blockID.y].end - meta[blockID.y].start;
    uint index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    uint localIndex = (index - meta[blockID.y].start) % amount;
    
    Input particle = inputParticleData[index];
    // -------------------------------------------------------------- // 

    
    // --- Set the standard stuff --- //
    float dt = meta[0].deltaTime;
    particle.time = particle.time + dt;
    particle.size = meta[blockID.y].size;
    particle.patterns = meta[blockID.y].pattern;
        //VFX Pattern is stored in morePositionInfo.y to save space
    particle.VFXpatterns = (int) meta[blockID.y].morePositionInfo.y;
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
        particle.position.x = particle.position.x + dirVec.x * (particle.velocity.x * 12.5f) * dt * meta[OneHundo_TwoFiveFive].deltaTime;
        particle.position.y = particle.position.y; // +(((float) DTid.x - 127) / 128) * dt;
        particle.position.z = particle.position.z + dirVec.y * (particle.velocity.z * 12.5f) * dt * meta[OneHundo_TwoFiveFive].deltaTime;

    }
    else
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);

        particle.position = startPosition;
        particle.time = 0.f;
        particle.size = 0.5f;
    }

    
    if (meta[blockID.y].pattern == 11) //Ice ice baby
    {
        particle.rgb = float3(.0f, .75f, .95f);
    }
    else
    {
        particle.rgb = float3(1.f, 0.10f, .0f);
    }
    
    outputParticleData[index] = particle;
}

void LightningMovement(in uint3 DTid, in uint3 blockID)
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
    particle.patterns = meta[blockID.y].pattern;
        //VFX Pattern is stored in morePositionInfo.y to save space
    particle.VFXpatterns = (int) meta[blockID.y].morePositionInfo.y;
    // ------------------------------ //
    
    
    float posy = localIndex * 0.2f; // 51 / 255
    float idxFraction = localIndex / 255.f;
    float timeFraction = PI * (1 - (particle.time / meta[blockID.y].life));
    
    float alpha = pow(sin(2 * PI * idxFraction + timeFraction), 3); // Pi
    float beta = pow(sin(4 * 2.71828f * idxFraction + 4 * timeFraction), 3); // Eulers
    float gamma = pow(sin(6 * sqrt(5) * idxFraction + 9 * timeFraction), 3); // Root(5)
    
    particle.position.y = posy;
    particle.position.x = (2 * alpha + beta + 2 * gamma);
    particle.position.z = (alpha + 2 * beta - gamma);
    
    particle.rgb.r = 0.0f;
    particle.rgb.g = 0.0f;
    particle.rgb.b = 1.0f;
        
    outputParticleData[index] = particle;
}

void SpiralFieldMovement(in uint3 DTid, in uint3 blockID)
{
    // -- Calculate the index and get the right particle to change -- //
    int amount = meta[blockID.y].end - meta[blockID.y].start;
    int index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x;
    int localIndex = (index - meta[blockID.y].start) % amount;
    
    Input particle = inputParticleData[index];
    
    float indexValue = sqrt((10.f + (float) localIndex) / (10.f + (float) amount));
    // --- Set the standard stuff --- //
    float dt = meta[0].deltaTime;
    particle.time = particle.time + dt;
    particle.size = meta[blockID.y].size;
    particle.patterns = meta[blockID.y].pattern;
        //VFX Pattern is stored in morePositionInfo.y to save space
    particle.VFXpatterns = (int) meta[blockID.y].morePositionInfo.y;
    // ------------------------------ //
    
    
    
    float timeValue = (particle.time / meta[blockID.y].life);
    // "Uneven" circlings with a bit over 8 laps
    float piFraction = ((indexValue) * PI * 50.f) + timeValue * timeValue;
    // Expanding outwards
    float radius = meta[blockID.y].maxRange * indexValue * sqrt(timeValue);
    
    //particle.size = meta[blockID.y].size * /*sqrt*/(1 - timeValue);
    
    particle.position.x = cos(piFraction) * radius;
    particle.position.y = timeValue * indexValue;
    particle.position.z = sin(piFraction) * radius;
    
    if (meta[blockID.y].morePositionInfo.x > 0)
    {
        particle.rgb.r = 0.0f;
        particle.rgb.g = 0.0f;
        particle.rgb.b = 1.0f;
    }
    else if (meta[blockID.y].morePositionInfo.y > 0)
    {
        particle.rgb.r = 1.0f;
        particle.rgb.g = 0.0f;
        particle.rgb.b = 0.0f;
    }

      
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
    particle.patterns = meta[blockID.y].pattern;
        //VFX Pattern is stored in morePositionInfo.y to save space
    particle.VFXpatterns = (int) meta[blockID.y].morePositionInfo.y;
    // ------------------------------ //
    
    
    // ---- Get a "randomized" value to access deltaTime ---- //    
    float psuedoRand = sin(index * 71.01) * sin(index * 71.01);
    
    float holder = frac(sin(dot(index, float2(12.9898, 78.233))) * 43758.5453) * 100.f;
    
    int One_OneHundo = holder;
    if (One_OneHundo == 0)
        One_OneHundo = 1;
    
    int OneHundo_TwoFiveFive = One_OneHundo + 155;
    // ------------------------------------------------------ //
            
    if (99999.f == particle.position.x, 99999.f == particle.position.y, 99999.f == particle.position.z)
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);
       
        particle.position = meta[blockID.y].startPosition;
        particle.time = 0.f;
    }
    if (particle.time >= meta[blockID.y].life)
    {
        float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);

        particle.position = startPosition;
        particle.time = 0.f;
    }

    float oscillationX = (meta[One_OneHundo].deltaTime + 1.f) * cos(2.f * PI * 2.5f * meta[OneHundo_TwoFiveFive].deltaTime * particle.time / meta[blockID.y].life + 0.5f * (float) index); //+ /*0.5f **/ index);
    float oscillationZ = (meta[One_OneHundo].deltaTime + 1.f) * sin(2.f * PI * 2.5f * meta[OneHundo_TwoFiveFive].deltaTime * particle.time / meta[blockID.y].life + 0.5f * (float) index); //+ /*0.5f **/index);
    
    float dirX = cos((float) localIndex / (float) amount * 15.0f) * 28/*30.f*/;
    float dirZ = sin((float) localIndex / (float) amount * 15.0f) * 28/*30.f*/;

    particle.size = (particle.size / (1 / particle.time));
        
    particle.position.x = particle.position.x + (oscillationX + dirX) * dt;
    particle.position.z = particle.position.z + (oscillationZ + dirZ) * dt;
    
    particle.rgb.r = 10.0f;
    particle.rgb.g = 0.1f;
    particle.rgb.b = 0.0f;
        
    outputParticleData[index] = particle;
}

void FireMovement(in uint3 DTid, in uint3 blockID)
{
    // -- SAME FOR ALL FUNCTIONS -- //
    uint amount = meta[blockID.y].end - meta[blockID.y].start; //particels
    uint index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x; //index slots of the 65000
    uint localIndex = (index - meta[blockID.y].start) % amount;
    
    Input particle = inputParticleData[index];
    // -------------------------------------------------------------- // 

    // --- Set the standard stuff --- //
    float dt = meta[0].deltaTime;
    particle.time = particle.time + dt;
    particle.size = meta[blockID.y].size;
    particle.patterns = meta[blockID.y].pattern;
        //VFX Pattern is stored in morePositionInfo.y to save space
    particle.VFXpatterns = (int) meta[blockID.y].morePositionInfo.y;
    // ------------------------------ //
   
    float holder = frac(sin(dot(index, float2(12.9898, 78.233))) * 43758.5453) * 100.f;
    
    int One_OneHundo = holder;
    if (One_OneHundo == 0)
        One_OneHundo = 1;
    int OneHundo_TwoFiveFive = One_OneHundo + 155;
    // ------------------------------------------------------ //
    
    float timeValue = (particle.time / meta[blockID.y].life);
    
     // ---- Get a "randomized" value to access deltaTime ---- //    
    //float psuedoRand = sin(index * 71.01) * sin(index * 71.01);
    //???float directionRandom = normalize(float((DTid.x % 5.0) / 5.0f - 0.5f));
    
    //Time differense for variation in animation 
    if (particle.time >= meta[blockID.y].life + meta[One_OneHundo].deltaTime)
    {
        particle.time = 0.0f;
    }
    else if (99999.f == particle.position.x, 99999.f == particle.position.y, 99999.f == particle.position.z)
    {
        particle.time = meta[index].deltaTime * meta[blockID.y].life;
        

    }
    
    //Set specified Start position 
    float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);
    particle.position = startPosition;

    float oddEvenFactor = ((index % 2) - 0.5f) * 2; //gives values 0 or 1 based on particle 
   
    particle.position.x = particle.position.x + (localIndex);
    particle.position.y = 1 + particle.position.y;
    particle.position.z = particle.position.z; // + oddEvenFactor * ( 4* meta[index].deltaTime);

    particle.rgb.r = 1.0f;
    particle.rgb.g = 1.0f;
    particle.rgb.b = 1.0f;
    
    outputParticleData[index] = particle;

}

void HotPotMovement(in uint3 DTid, in uint3 blockID)
{
    // -- SAME FOR ALL FUNCTIONS -- //
    uint amount = meta[blockID.y].end - meta[blockID.y].start; //particels
    uint index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x; //index slots of the 65000
    uint localIndex = (index - meta[blockID.y].start) % amount;
    
    Input particle = inputParticleData[index];
    // -------------------------------------------------------------- // 

    // --- Set the standard stuff --- //
    float dt = meta[0].deltaTime;
    particle.time = particle.time + dt; //
    particle.size = meta[blockID.y].size;
    particle.patterns = meta[blockID.y].pattern;
        //VFX Pattern is stored in morePositionInfo.y to save space
    particle.VFXpatterns = (int) meta[blockID.y].morePositionInfo.y;
    // ------------------------------ //
   
    
    float holder = frac(sin(dot(index, float2(12.9898, 78.233))) * 43758.5453) * 100.f;
    
    int One_OneHundo = holder;
    if (One_OneHundo == 0)
        One_OneHundo = 1;
    int OneHundo_TwoFiveFive = One_OneHundo + 155;
    // ------------------------------------------------------ //
    
    float timeValue = (particle.time / meta[blockID.y].life);
    
     // ---- Get a "randomized" value to access deltaTime ---- //    
    //float psuedoRand = sin(index * 71.01) * sin(index * 71.01);
    //???float directionRandom = normalize(float((DTid.x % 5.0) / 5.0f - 0.5f));
    
    //Time differense for variation in animation 
    if (particle.time >= meta[blockID.y].life + meta[One_OneHundo].deltaTime)
    {
        particle.time = 0.0f;
    }
    else if (99999.f == particle.position.x, 99999.f == particle.position.y, 99999.f == particle.position.z)
    {
        particle.time = meta[index].deltaTime * meta[blockID.y].life;
        

    }
    
    //Set specified Start position 
    float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);
    particle.position = startPosition;

    float oddEvenFactor = ((index % 2) - 0.5f) * 2; //gives values 0 or 1 based on particle index
    particle.size = particle.size + (1*oddEvenFactor);
    particle.position.x = particle.position.x + (index*2/*+4*/);
    particle.position.y = 1 + particle.position.y;
    particle.position.z = particle.position.z;
    //particle.position.z = particle.position.z + index + (oddEvenFactor * ( /*4*/6 * meta[index].deltaTime));

    particle.rgb.r = 1.0f;
    particle.rgb.g = 0.0f;
    particle.rgb.b = 0.0f;
    
    outputParticleData[index] = particle;
}

void SparkMovement(in uint3 DTid, in uint3 blockID)
{
    // -- SAME FOR ALL FUNCTIONS -- //
    uint amount = meta[blockID.y].end - meta[blockID.y].start; //particels
    uint index = meta[blockID.y].start + blockID.x * NUM_THREADS + DTid.x; //index slots of the 65000
    uint localIndex = (index - meta[blockID.y].start) % amount;
    
    Input particle = inputParticleData[index];
    // -------------------------------------------------------------- // 

    // --- Set the standard stuff --- //
    float dt = meta[0].deltaTime;
    particle.time = particle.time + dt; 
    particle.size = meta[blockID.y].size;
    particle.patterns = meta[blockID.y].pattern;
        //VFX Pattern is stored in morePositionInfo.y to save space
    particle.VFXpatterns = (int) meta[blockID.y].morePositionInfo.y;
    // ------------------------------ //
   
    
    float holder = frac(sin(dot(index, float2(12.9898, 78.233))) * 43758.5453) * 100.f;
    
    int One_OneHundo = holder;
    if (One_OneHundo == 0)
        One_OneHundo = 1;
    int OneHundo_TwoFiveFive = One_OneHundo + 155;
    // ------------------------------------------------------ //
    
    float timeValue = (particle.time / meta[blockID.y].life);
    
     // ---- Get a "randomized" value to access deltaTime ---- //    
    //float psuedoRand = sin(index * 71.01) * sin(index * 71.01);
    //???float directionRandom = normalize(float((DTid.x % 5.0) / 5.0f - 0.5f));
    
    //Time differense for variation in animation 

    if (99999.f == particle.position.x, 99999.f == particle.position.y, 99999.f == particle.position.z)
    {
        particle.time = meta[index].deltaTime * meta[blockID.y].life;
        

    }
    else if (particle.time >= meta[blockID.y].life + meta[One_OneHundo].deltaTime)
    {
        particle.time = 0.0f;
    }
    
    
    //Set specified Start position 
    float3 startPosition = float3(meta[blockID.y].startPosition.x, meta[blockID.y].startPosition.y, meta[blockID.y].startPosition.z);
    particle.position = startPosition;

    float oddEvenFactor = ((index % 2) - 0.5f) * 2; //gives values 0 or 1 based on particle index
    particle.size = particle.size + (1 * oddEvenFactor);
    if (index %2 == 1)
    {
        particle.position.x = (particle.position.x + ((index / meta[blockID.y].maxRange) * meta[blockID.y].maxRange) * -1);
    }
    else
    {
        particle.position.x = (particle.position.x + (index / meta[blockID.y].maxRange) * meta[blockID.y].maxRange);

    }
  
    particle.position.y = particle.position.y + (index * particle.time);
    //////fire waterfall ??  particle.position.y = particle.position.y + (index / particle.time);
    particle.position.z = particle.position.z  /*+ (oddEvenFactor * ( 6 * meta[index].deltaTime)*/;
    
    float travelledDistance = distance(particle.position, meta[blockID.y].startPosition);
    
    //if (travelledDistance >= (meta[blockID.y].maxRange + 1))
    //{
    //    particle.position = startPosition;
    //    particle.time = 0.f;
    //}
  
    particle.rgb.r = 1.0f;
    particle.rgb.g = 0.0f;
    particle.rgb.b = 0.0f;
    
    outputParticleData[index] = particle;
}

void NoMovement(in uint3 DTid, in uint3 blockID)
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
    particle.patterns = meta[blockID.y].pattern;
        //VFX Pattern is stored in morePositionInfo.y to save space
    particle.VFXpatterns = (int) meta[blockID.y].morePositionInfo.y;
    // ------------------------------ //
    
    
    particle.position.x = meta[blockID.y].startPosition.x;
    particle.position.y = meta[blockID.y].startPosition.y;
    particle.position.z = meta[blockID.y].startPosition.z;
        
    outputParticleData[index] = particle;
}
