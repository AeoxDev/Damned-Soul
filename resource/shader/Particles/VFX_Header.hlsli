// NOTE: REQUIRED in the pixel shader for Alpha Blending
//float4 backBuffer = SampleBackbuffer(gs_in.position, screenResolution_in, backbufferTexture_in, vfxSampler_in);


// Function to UV Pan coordinates according to constant buffer values and based on time.
float2 UVPan(in float2 offsetXY, in float panSpeed, in float time, in float2 originalUVCoordinates)
{
    return float2(originalUVCoordinates.x + frac(offsetXY.x * panSpeed * time),
                  originalUVCoordinates.y + frac(offsetXY.y * panSpeed * time));
}

// Function to rotate the UV coordinates around the center.
float2 UVRotate(in float2 ogUVCoord, in float angle, in float scaleTexture) // NOTE: REQUIRES WRAPPING TEXTURES, NO CLIPPING
{
    float x = (ogUVCoord.x - (0.5f * scaleTexture)) * cos(angle) - (ogUVCoord.y - (0.5f * scaleTexture)) * sin(angle);
    float y = (ogUVCoord.x - (0.5f * scaleTexture)) * sin(angle) + (ogUVCoord.y - (0.5f * scaleTexture)) * cos(angle);
    
    return float2(x, y);
}

// Gets the screen space derivatives. Used to overwrite mipmaps.
float4 UVddxddy(in float2 uv)
{
    return float4(ddx(uv), ddy(uv));
}

// Function to produce polar coordinates.
float2 UVPolarCoordinates(in float2 ogUVCoord, in float radius, in float rotation, in float scaleTexture = 1.0f)
{
    float2 uv = (ogUVCoord * scaleTexture - (0.5f * scaleTexture)) * 2.0f;
    
    float r = length(uv) + frac(radius);
    float theta = atan2(uv.y, uv.x) + rotation;
    
    float2 polarCoord = float2(theta / radians(360.0f), r);
    return frac(polarCoord.xy);
}

// Easier function for who want to do polarCoordinates but doesnt know the workflow.
float4 SamplePolarCoordinateTexture(in Texture2D polarTexture, in sampler polarSampler, in float time, in float2 uv, in float rotationSpeed = 0.0f, in float panSpeed = 0.0f, in float scaleTexture = 1.0f)
{
    float4 dd = UVddxddy(uv);
    return polarTexture.SampleGrad(polarSampler, UVPolarCoordinates(uv, time * panSpeed, time * rotationSpeed, scaleTexture), dd.xy, dd.zw);
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

// Alpha blending with the backbuffer. Note that the "diffuse texture" can be manipulated per color channel, and could also just be a mask.
float4 AlphaBlend(in float4 backBuffer, in float alphaMaskOrValue, in float3 diffuseTexture, in float3 colorValue = float3(1.0f, 1.0f, 1.0f))
{
    float4 blendResults = {
        (backBuffer.r * (1 - alphaMaskOrValue.r) + (diffuseTexture.r * colorValue.r * alphaMaskOrValue.r)),
        (backBuffer.g * (1 - alphaMaskOrValue.r) + (diffuseTexture.g * colorValue.g * alphaMaskOrValue.r)),
        (backBuffer.b * (1 - alphaMaskOrValue.r) + (diffuseTexture.b * colorValue.b * alphaMaskOrValue.r)),
        1.0f };

    return float4(saturate(blendResults.rgb), 1.0f);
}

// Samples from the color gradient texture.
float3 SampleColorRamp(in Texture2D colorRampTexture, in sampler samplerState, in float4 grayScaleTexture)
{    
    return colorRampTexture.Sample(samplerState, float2(clamp(grayScaleTexture.x, 0.01f, 0.99f), 0.5f)).rgb;
}