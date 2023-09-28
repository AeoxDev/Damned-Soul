struct Input
{
    float3 position;
    float padding1;
    float3 velocity;
    float padding2;
    float3 rgb;
    float padding3;
    
};

cbuffer metadata : register(b0)
{
    int start;
    int end;
    float life;
    float maxRange;
    float3 startPosition;
};

StructuredBuffer<Input> inputParticleData : register(t0);
RWStructuredBuffer<Input> outputParticleData : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    Input particle = inputParticleData[DTid.x];
    //____________________________________________________________________
    float speedOfProjectile = 8.0f; // adjust as you see fit 
    
    //this is in radians, basically the spread of the cone
    float angleOfCone = 45.0f * (3.14159f / 180.0f); // Convert degrees to radians. Feel free to change angle (first number) as you see fit
    
    float radianAngle = ((float) DTid.x / float(end) - 0.5f) * angleOfCone;
    float3 direction = normalize(float3(cos(radianAngle), -1.0f, sin(radianAngle)));
    
    particle.position += direction * speedOfProjectile;

    // Reset particles that have moved out of range to the starting position
    if (particle.position.y < 0.0f)
    {
        particle.position = startPosition;
    }
    //____________________________________________________________________
    //test.position = test.position;
    outputParticleData[DTid.x] = particle;
}