cbuffer WorldMatrix : register(b0)
{
    matrix world;
}

cbuffer CameraBuffer : register(b1)
{
    float4 cameraPosition;
    matrix view;
    matrix projection;
}

struct metadata
{
    int start;
    int end;
    float life; // How much time (in seconds) a particle is allowed to live
    float maxRange; // How long particle is allowed to go 
    float3 startPosition;
    float size; // Size of particle
    float3 positionInfo;
    int pattern; // The movement pattern of the particle and decidor of size
    
    float deltaTime;
    // Slot 0 is reserved for delta time
    // Slot 1-99 is a random number between 0.0 and 1.0
    // Slot 100-255 is a random number between 1.0 and 5.0
    
    float2 morePositionInfo;
    bool reset;
};


cbuffer metadataBuffer : register(b2)
{
    metadata meta[256];
};

cbuffer metadataBuffer : register(b3)
{
    int metadataSlot;
    float timeAlive;
};


struct Input
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 uv : UV;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV;
    float time : TIME;
    int patterns : PATTERNS;
    int VFXpatterns : VFXPATTERNS;
};

VS_OUT main(Input inval)
{
    VS_OUT retval;

    
    matrix worldView = mul(world, view);
    //matrix world;

    // Extract rotation matrix from the world matrix
    float3x3 rotationMatrix = (float3x3) world;
    //rotationMatrix[2] = float3(0.f, 0.f, 1.f);
    //rotationMatrix[1].z = 0.f;
    //rotationMatrix[0].z = 0.f;

//    // Extract rotation (as Euler angles)
//    float3 rotation;
//    rotation.x = atan2(rotationMatrix[1].z, rotationMatrix[1].y);
//    rotation.y = atan2(-rotationMatrix[2].z, sqrt(rotationMatrix[2].x * rotationMatrix[2].x + rotationMatrix[2].y * rotationMatrix[2].y));
//    rotation.z = atan2(rotationMatrix[2].y, rotationMatrix[2].x);

//    // Rotate only in the Z axis
//    // Assuming you want to rotate by an additional 45 degrees
//    float angleInDegrees = 0.0f;
//    float angleInRadians = radians(angleInDegrees);

//    // Create a rotation matrix for the additional Z-axis rotation
//    float2x2 additionalRotationMatrix = float2x2(
//    cos(angleInRadians), -sin(angleInRadians),
//    sin(angleInRadians), cos(angleInRadians)
//    );

//    // Apply additional Z-axis rotation to the original rotation matrix
//    rotationMatrix = float3x3(
//    rotationMatrix[0].x * additionalRotationMatrix[0].x + rotationMatrix[0].y * additionalRotationMatrix[1].x,
//    rotationMatrix[0].x * additionalRotationMatrix[0].y + rotationMatrix[0].y * additionalRotationMatrix[1].y,
//    0,

//    rotationMatrix[1].x * additionalRotationMatrix[0].x + rotationMatrix[1].y * additionalRotationMatrix[1].x,
//    rotationMatrix[1].x * additionalRotationMatrix[0].y + rotationMatrix[1].y * additionalRotationMatrix[1].y,
//    0,

//    rotationMatrix[2].x,
//    rotationMatrix[2].y,
//    1
//);


//    // 3. Billboard the model after the rotation in Z axis
//    // Assuming your input position is in local space
//    float3 rotatedPosition = mul(inval.position.xyz, rotationMatrix);

//    // Combine the rotated position with the original z component
//    float3 finalPosition = rotatedPosition + world[3].xyz;

//    // Transform to world space
//    float4 worldPosition = mul(float4(finalPosition, 1.0f), world);

//    // Transform to view space
//    float4 viewPosition = mul(worldPosition, view);

//// Perform the multiplication with the projection matrix
//    float4 projectedPosition = mul(viewPosition, projection);
//    projectedPosition /= projectedPosition.w;

//// Output the final positions
//    retval.position = projectedPosition;
            
    
    retval.position = float4(mul((inval.position.xyz * meta[metadataSlot].size), rotationMatrix), 1.0f);
    retval.position = mul((float4(retval.position.xyz, 1.0f) + float4(worldView[3].xyz, 0.0f)), projection);
    retval.position /= retval.position.w;

    //retval.position = mul((float4(inval.position.xyz, 1.0f) + float4(worldView[3].xyz, 0.0f)), projection);
    //retval.position /= retval.position.w;

    




    //worldView = mul(worldView, projection);
    //retval.position = mul(float4(inval.position.xyz, 1.0f), worldView);

    
    retval.rgb = float4(1.f, 1.f, 1.f, 1.f);
    retval.uv = inval.uv;
    retval.time = timeAlive;
    retval.patterns = meta[metadataSlot].pattern;
    // VFX pattern is stored in morePositionInfo.y to save space
    retval.VFXpatterns = meta[metadataSlot].morePositionInfo.y;
    //retval.worldPosition = mul(retval.position, world);
    //retval.rotationZ = inval.rotationZ;
    //retval.size = inval.size;

    return retval;
}