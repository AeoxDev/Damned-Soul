#include "VFX_Header.hlsli"

sampler WrapSampler : register(s3);
Texture2D backbufferTexture_in : register(t0);

Texture2D colorRampTexture_in : register(t1);
Texture2D noiseTexture_in : register(t2); // R: VORNOI        | G: GAUSSIAN NOISE | B: CAUSTIC NOISE  | A: - |
Texture2D shapeTexture_in : register(t3); // R: SOFTCIRCLE    | G: SHARPERCIRCLE  | B: -              | A: - |
Texture2D maskTexture_in : register(t4); // R: GRADIENT      | G: SWORDGRADIENT  | B: -              | A: - |

cbuffer pannerInput : register(b0)
{
    float2 screenResolution_in; // How big is the screen resolution?
};

// ######## PREDEFINED VFX BEHAVIORS, TINY TWEAKABILITY #########


// FIRE REQUIRES: VFX_Noises, VFX_Shapes & VFX_FireGradient
float4 VFXFire(
in float4 backBuffer,
in float time,
in float2 uv,
in float fireMultiplier = 1.0f,
in float3 fireColor = float3(0.0f, 0.0f, 0.0f)
)
{
    float vornoiTexture = noiseTexture_in.Sample(WrapSampler, UVPan(float2(0.0f, 1.0f), 0.750f, time, uv * 0.5f)).r;
    float gNoiseTexture = noiseTexture_in.Sample(WrapSampler, UVPan(float2(0.0f, 1.0f), 0.675f, time, uv)).g;
    float vornoiDiffuse = noiseTexture_in.Sample(WrapSampler, UVPan(float2(0.0f, 1.0f), 0.750f, time, uv)).r;
    
    
    float2 distortedUV = distortUV(0.1f, distortUV(0.132f, uv, pow(vornoiTexture, 1.2f)), gNoiseTexture);
    float4 fireDistort = shapeTexture_in.Sample(WrapSampler, distortedUV).r;
    float3 color = SampleColorRamp(colorRampTexture_in, WrapSampler, fireDistort);
    
    return AlphaBlend(backBuffer, fireDistort.r, color + (fireColor * fireMultiplier));
}

// ACID REQUIRES: VFX_Noises, VFX_Shapes & VFX_FireGradient
float4 VFXAcidSpit(
in float4 backBuffer,
in float time,
in float2 uv
)
{
    // To change the speed and direction of the rotations, change what time is multiplied with.   
    float4 distortVornoi = noiseTexture_in.Sample(WrapSampler, UVRotate(uv, time * -15.6f, 0.8f)).r;
    float2 vornoiUV = distortUV(0.1f, uv, distortVornoi);
    
    float4 panningVornoi = noiseTexture_in.Sample(WrapSampler, UVRotate(uv, time * 15.6f, 0.65f)).r;
    
    float4 distortedProjectile = shapeTexture_in.Sample(WrapSampler, vornoiUV).g;
    float4 newProjectile = distortedProjectile - (panningVornoi * distortedProjectile);
    
    float4 colorizeProjectile = lerp((newProjectile - float4(0.0f, 0.0f, 1.05f, 1.0f)), (newProjectile - float4(1.0f, 0.0f, 1.0f, 1.0f)), 0.18f);
    
    return AlphaBlend(backBuffer, distortedProjectile.r, colorizeProjectile.rgb);
}

// REQUIRES: VFX_Noises & VFX_Shapes
float4 VFXAcidGround(
in float4 backBuffer,
in float time,
in float2 uv
)
{
// Creates the Texture Patterns and Colorizes them using subtract.
    float4 panVornoi = (float4(0.0f, 1.0f, 0.0f, 1.0f) - SamplePolarCoordinateTexture(noiseTexture_in, WrapSampler, time, uv, 0.0f, -0.05f, 0.2f).r);
    float4 rotateVornoi = (float4(1.0f, 1.0f, 0.0f, 1.0f) * SamplePolarCoordinateTexture(noiseTexture_in, WrapSampler, time, uv, 0.15f, -0.01f, 0.3f).r);
    float4 colorizedAcid = lerp(panVornoi, rotateVornoi, 0.75f);
    
// Creates the Alpha Mask
    float alphaMask = shapeTexture_in.Sample(WrapSampler, distortUV(0.12f, uv, noiseTexture_in.Sample(WrapSampler, uv).r)).g;
    alphaMask = clamp(2.0f * alphaMask, 0.0f, 1.0f);
    
 // Alpha Blends
    return AlphaBlend(backBuffer, alphaMask, colorizedAcid.rgb);
}


// REQUIRES: VFX_Noises & VFX_Shapes
float4 VFXFireBall(
in float4 backBuffer,
in float time,
in float2 uv
)
{
    // Pan the distortion Textures
    float panGNoise = noiseTexture_in.Sample(WrapSampler, UVRotate(uv, time * 15.6f, 0.8f)).g;
    float panVornoi = noiseTexture_in.Sample(WrapSampler, UVRotate(uv, time * -5.0f, 0.5f)).r;
    
    // Distort UV channels
    float2 fireUV = lerp(lerp(panGNoise, panVornoi, -0.31f), uv, 0.9f);
    
    // Create Alpha Mask
    float alphaMask = shapeTexture_in.Sample(WrapSampler, fireUV).g;
    
    // Mask magic
    float4 subtractionMask = (alphaMask - (alphaMask * panVornoi)) * float4(1.0f, 0.6f, 0.0f, 0.0f);
    float4 multiplicationMask = (alphaMask - (alphaMask * (panVornoi * 0.49f))) - float4(0.0f, 0.43f, 1.0f, 1.0f);
    
    // Colors the projectile
    float4 colorizedProjectile = subtractionMask + multiplicationMask;
    
    return AlphaBlend(backBuffer, alphaMask, colorizedProjectile.rgb);
}

// REQUIRES: VFX_Noises & VFX_Shapes
float4 VFXFireBallEmpowered(
in float4 backBuffer,
in float time,
in float2 uv
)
{
    // Pan the distortion Textures
    float panGNoise = noiseTexture_in.Sample(WrapSampler, UVRotate(uv, time * 15.6f, 0.8f)).g;
    float panVornoi = noiseTexture_in.Sample(WrapSampler, UVRotate(uv, time * -5.0f, 0.5f)).r;
    
    // Distort UV channels
    float2 fireUV = lerp(lerp(panGNoise, panVornoi, -0.31f), uv, 0.9f);
    
    // Create Alpha Mask
    float alphaMask = shapeTexture_in.Sample(WrapSampler, fireUV).g;
    
    // Mask magic
    float4 subtractionMask = (alphaMask - (alphaMask * panVornoi)) * float4(0.0f, 0.75f, 1.0f, 0.0f);
    float4 multiplicationMask = (alphaMask - (alphaMask * (panVornoi * 0.49f))) - float4(0.3f, 0.34f, 0.0f, 1.0f);
    
    // Colors the projectile
    float4 colorizedProjectile = multiplicationMask + subtractionMask;
    
    return AlphaBlend(backBuffer, alphaMask, colorizedProjectile.rgb);
}

// REQUIRES: VFX_NOISES & VFX_SHAPES
float4 VFXSpawnCrystals(
in float4 backBuffer,
in float time,
in float2 uv,
float dissippateSpeed = 0.25f
)
{
    // NOTE: IF YOU WANT TO MAKE IT GO FASTER, INCREASTE THIS VALUE
    // IMPORTANT: MATCH THE PARTICLE LIFETIME OR THE ANIMATION WILL BE REPEATING INFINITELY.
    float dissippateValue = dissippateSpeed;
    
    // The panning textures
    float4 panCaustics = (float4(0.0f, 0.6666667f, 1.0f, 1.0f) * (3.76f * SamplePolarCoordinateTexture(noiseTexture_in, WrapSampler, time, uv, 0.04f, -0.25f, 0.39f).b));
    float4 rotateCaustics = (float4(0.3215686f, 0.0f, 1.0f, 1.0f) * (4.23f * SamplePolarCoordinateTexture(noiseTexture_in, WrapSampler, time, uv, 0.5f, -0.21f, 0.34f).b));
    
    // Colorizing
    float4 colorizedSpawn = panCaustics + rotateCaustics;
    
    //Alpha mask
    float alphaMask = shapeTexture_in.Sample(WrapSampler, uv * (1.0f - clamp(time * dissippateValue, 0.0f, 1.0f))).g;
    
    // Making Alpha disappear
    float timedMask = alphaMask * shapeTexture_in.Sample(WrapSampler, uv).g;
    
    return AlphaBlend(backBuffer, timedMask, colorizedSpawn.rgb);
}

// REQUIRES: VFX_NOISES & VFX_SHAPES
float4 VFXSpawnImp(
in float4 backBuffer,
in float time,
in float2 uv,
in float dissippateSpeed = 0.9f
)
{
    // NOTE: IF YOU WANT TO MAKE IT GO FASTER, INCREASTE THIS VALUE
    // IMPORTANT: MATCH THE PARTICLE LIFETIME OR THE ANIMATION WILL BE REPEATING INFINITELY.
    float dissippateValue = dissippateSpeed;
    
    // The panning textures
    float4 panCaustics = (float4(1.0f, 0.7675428f, 0.0f, 0.0f) * (3.76f * SamplePolarCoordinateTexture(noiseTexture_in, WrapSampler, time, uv, 0.04f, -0.25f, 0.39f).b));
    float4 rotateCaustics = (float4(1.0f, 0.1011105f, 0.0f, 0.0f) * (4.23f * SamplePolarCoordinateTexture(noiseTexture_in, WrapSampler, time, uv, 0.5f, -0.21f, 0.34f).b));
    
    // Colorizing
    float4 colorizedSpawn = panCaustics + rotateCaustics;
    
    //Alpha mask
    float alphaMask = shapeTexture_in.Sample(WrapSampler, uv * sin(time * dissippateValue)).g;
    
    // Making Alpha disappear
    float timedMask = alphaMask * shapeTexture_in.Sample(WrapSampler, uv).g;
    
    return AlphaBlend(backBuffer, timedMask, colorizedSpawn.rgb);
}

// REQUIRES: VFX_NOISES & VFX_SHAPES
float4 VFXSpawnEmpoweredImp(
in float4 backBuffer,
in float time,
in float2 uv,
in float dissippateSpeed = 0.9f
)
{
    // NOTE: IF YOU WANT TO MAKE IT GO FASTER, INCREASTE THIS VALUE
    // IMPORTANT: MATCH THE PARTICLE LIFETIME OR THE ANIMATION WILL BE REPEATING INFINITELY.
    float dissippateValue = dissippateSpeed;
    
    // The panning textures
    float4 panCaustics = (float4(0.2171591f, 0.754717f, 0.6784216f, 0.0f) * (3.76f * SamplePolarCoordinateTexture(noiseTexture_in, WrapSampler, time, uv, 0.04f, -0.25f, 0.39f).b));
    float4 rotateCaustics = (float4(0.6100035f, 0.843265f, 0.8679245f, 0.0f) * (4.23f * SamplePolarCoordinateTexture(noiseTexture_in, WrapSampler, time, uv, 0.5f, -0.21f, 0.34f).b));
    
    // Colorizing
    float4 colorizedSpawn = panCaustics + rotateCaustics;
    
    //Alpha mask
    float alphaMask = shapeTexture_in.Sample(WrapSampler, uv * sin(time * dissippateValue)).g;
    
    // Making Alpha disappear
    float timedMask = alphaMask * shapeTexture_in.Sample(WrapSampler, uv).g;
    
    return AlphaBlend(backBuffer, timedMask, colorizedSpawn.rgb);
}

// PORTAL REQUIRES: VFX_Noises
float4 VFXPortal(
float4 backBuffer,
float time,
float2 uv
)
{
    // Distorts UV
    float vornoiDistortion = noiseTexture_in.Sample(WrapSampler, UVPan(float2(0.0f, -1.0f), 0.2f, time, uv * 1.1f)).r;
    
    // Colorizes
    float4 colorizedCaustic = (noiseTexture_in.Sample(WrapSampler, distortUV(0.1f, uv, vornoiDistortion)).b * float4(0.6171079f, 0.4386792f, 1.0f, 0.0f));
    float4 colorizedGaussian = (noiseTexture_in.Sample(WrapSampler, uv * 0.4f).g * float4(0.3818085f, 0.3938224f, 0.490566f, 0.0f));
    
    // Additive Color
    float4 colorizedPortal = colorizedCaustic + colorizedGaussian;
    
    //Display
    return float4(colorizedPortal.rgb, 1.0f); //(gMaskTexture * pow(vornoiTexture, 2.0f + sin(time))).rgb, 1.0f;
}

// SWORDSSLASH REQUIRES: VFX_Noises & VFX_Masks
float4 VFXSwordSlash(
float4 backBuffer,
float time,
float2 uv
)
{
    float4 vornoiTexture = pow(1.0f - noiseTexture_in.Sample(WrapSampler, UVPan(float2(1.0f, 0.0f), 0.25f, time, float2(uv.x * 2.35f, uv.y * -0.18))).r, -2.0f);
    float2 noiseTexture = noiseTexture_in.Sample(WrapSampler, uv).rg;
    
    float4 colorizedNoise = vornoiTexture * ( noiseTexture.r * float4(1.0f, 0.2066951f, 0.0f, 0.0f));
    
    float4 vornoiMask = pow(noiseTexture_in.Sample(WrapSampler, uv * 1.05f).r, time * 5.0f);
    float alphaMask = noiseTexture.g * vornoiMask.r;
    
    
    return AlphaBlend(backBuffer, alphaMask, colorizedNoise.rgb); //(gMaskTexture * pow(vornoiTexture, 2.0f + sin(time))).rgb, 1.0f;

}