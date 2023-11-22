// Function to UV Pan coordinates according to constant buffer values and based on time.
float2 UVPan(in float2 offsetXY, in float panSpeed, in float time, in float2 originalUVCoordinates)
{
    return float2(originalUVCoordinates.x - frac(offsetXY.x * panSpeed * time),
                originalUVCoordinates.y - frac(offsetXY.y * panSpeed * time));
}

// Function to distort the UV based on an existing texture.
float2 distortUV(in float distortionAmount, in float2 uvCoordinates, in float4 distortionTexture)
{
    float4 distortedTexture = lerp(float4(uvCoordinates.x, uvCoordinates.y, 0.0f, 0.0f), distortionTexture, distortionAmount);
    return distortedTexture.xy;
}

// Backbuffer Sampling for the UV of the particle
float4 SampleBackbuffer(in float4 position, in float2 screenResolution, in Texture2D backBufferTexture, in sampler samplerUsed)
{
    float2 screenSpaceUV = float2((position.x / screenResolution.x), (position.y / screenResolution.y));
    return backBufferTexture.Sample(samplerUsed, screenSpaceUV);

}