#include "RenderPipelineHeader.hlsli"

Texture2D diffuseTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D emissionTex : register(t2);
Texture2D depthTexture : register(t3);
Texture2D shadowTexture : register(t4);

SamplerState WrapSampler : register(s0);
SamplerState ShadowClampSampler : register(s1);

#define LIGHT_COMPONENT_ARRAY_LIMIT 64

struct LightComponent
{
    int type; // 1 pointlight, 2 spotlight
    float fallofFactor; //attenuation
    float lightRange;
    float lightCone;
    float4 lightColor;
    float4 lightDirection;
    float4 lightPosition;
};


cbuffer LightComponentShaderBuffer : register(b2)
{
    int firstLight; //First light in array
    int lastLight; //Last light in array
    float gammaCorrection; //Gamma correction
    float padding2;
    float4 dirLightColor;
    float4 dirLightDirection;
    float4 colorMultiplier;
    float4 colorAdditive;
    LightComponent lights[LIGHT_COMPONENT_ARRAY_LIMIT];
};

//cbuffer PS_POINT_LIGHT : register(b3)
//{
//    float4 pointLightColor;
//    float4 pointLightPosition;
//    float4 pointLightRange;
//};

//cbuffer PS_SPOT_LIGHT : register(b4)
//{
//    float4 spotLightColor;
//    float4 spotLightDirection; //normalize
//    float4 spotLightPosition;
//    float4 spotLightRange;
//    float4 spotLightCone;
//};

//cbuffer PS_MATERIAL : register(b5)
//{
//    float4 materialAmbient;
//    float4 materialDiffuse;
//    float4 materialSpecular;
//    float4 materialShininess;
//};

//struct PS_IN
//{
//    float4 position : SV_POSITION; //world, view, projection - multiplyed //Position on screen
//    float4 normal : WNORMAL; // world - multiplyed
//    float2 uv : UV;
//    float4 camToWorldObject : CAM; // normalized 
//    float4 world : WORLD;
//};

//struct PS_OUT
//{
//    float4 position : SV_Target0;
//    float4 normal : SV_Target1;
//    float4 SamplerColor : SV_Target2;

//};

float4 main(GS_OUT input) : SV_TARGET
{
    
    if (depthTexture.Load(input.base.position.xyz).w >= input.base.position.w + 0.0001f)
    {
        clip(-1.0f);
    }
    
    // Calculate normal based on normal map combined with true normal
    
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        input.tbn[i] = normalize(input.tbn[i]);
    }
    // No need to multiply by two when normalizing sicne the relative sizes are the same?
    float3 trueNormal = mul((normalTex.Sample(WrapSampler, input.base.uv).xyz * 2.f) - 1.f, input.tbn);
  
    float4 materialAmbient = { 0.02f, 0.02f, 0.02f, 1.0f }; //temp before material buffer
    float4 materialDiffuse = { 0.2f, 0.2f, 0.2f, 1.0f };
    float4 materialSpecular = { 0.2f, 0.2f, 0.2f, 1.0f };
    float4 materialShininess = { 8.0f, 0.0f, 0.0f, 1.0f };

    float4 image = diffuseTex.Sample(WrapSampler, input.base.uv); //texturImage
    clip(image.a - 0.1f);
    //Ambient, diffuse, specular
    float3 addOnColor = materialAmbient.xyz; //Ambient //lighting-effects to apply to texture
    float3 diffuse = materialDiffuse.xyz; //Sum of all diffuse lights
    float3 specular = materialSpecular.xyz; //Sum of all specular lights

            ///xxxx DirectionalLight xxxx///
    float3 diffuseDir = { 0.0f, 0.0f, 0.0f };
    float3 dirReflection = { 0.0f, 0.0f, 0.0f };
    float3 dirSpecular = { 0.0f, 0.0f, 0.0f };
    
        ///xxxx PointLight xxxx///
    float3 diffusePoint = { 0.0f, 0.0f, 0.0f };
    float3 pointReflection = { 0.0f, 0.0f, 0.0f };
    float3 pointSpecular = { 0.0f, 0.0f, 0.0f };
    
        ///xxxx SpotLight xxxx///
    float3 diffuseSpot = { 0.0f, 0.0f, 0.0f };
    float3 spotReflection = { 0.0f, 0.0f, 0.0f };
    float3 spotSpecular = { 0.0f, 0.0f, 0.0f };
    
    //Only do directional if not in shadow
    //Shadowmapping
    //Take world and move it to shadow texture
    float4 worldToTexture = float4(input.base.world.x, input.base.world.y, input.base.world.z, input.base.world.w);
    //float4 lastRow = shadowProjection._41_42_43_44;
    //lastRow = lastRow;
    worldToTexture = mul(worldToTexture, shadowView);
    worldToTexture = mul(worldToTexture, shadowProjection);
    worldToTexture.xyz /= worldToTexture.w;
    float depth = worldToTexture.z;

    //Convert to UV (0 to 1)
    float2 samplePos = float2((worldToTexture.x * 0.5f) + 0.5f, (-worldToTexture.y * 0.5f) + 0.5f);
   

    ///xxxx DirectionalLight Calculations including Shadows xxxx///

    //Shadow map is 1024
    int2 loadPos = (int) (1024.0f * samplePos.xy);
    float shadowDepth = shadowTexture.Sample(ShadowClampSampler, samplePos.xy).x;
    float shadowStrength = 0.5f;
    
    if (depth < shadowDepth + 0.0025f || shadowDepth == 0.0f) //Closer when 0
    {
        shadowStrength = 1.0f;
    //float shadowDepth = shadowTexture.Load(int3(loadPos.x, loadPos.y, 0.0f));
    }
        float3 invertedDirLightDirection = -dirLightDirection.xyz; //rätt?
        float dirLightIntesity = saturate(dot(trueNormal, invertedDirLightDirection));
        if (dirLightIntesity > 0.0f)
        {
            diffuseDir += shadowStrength * (dirLightColor * dirLightIntesity).rgb;
            saturate(diffuseDir);

            dirReflection = normalize(2 * dirLightIntesity * trueNormal - invertedDirLightDirection);
            dirSpecular = dirLightColor.xyz;
            dirSpecular *= shadowStrength * pow(saturate(dot(dirReflection * materialSpecular.xyz, input.base.camToWorldObject.xyz)), materialShininess.x);
            specular += dirSpecular;
        }
   
    for (int i = 0; i < LIGHT_COMPONENT_ARRAY_LIMIT; i++)
    {
        if (lights[i].type != 0)
        {
            //Point light
            if (lights[i].type == 1)
            {
                ///xxxx PointLight xxxx///
    
                float3 pixelToPointLightVector = normalize(lights[i].lightPosition.xyz - input.base.world.xyz); //L.. point in scene to pointLightsource 
                float pointLightIntesity = saturate(dot(trueNormal, pixelToPointLightVector.xyz)); // dot(normal,lightvector) givs angle inbetween, normal -pointLight
                float distanceToPointLight = length(input.base.world.xyz - lights[i].lightPosition.xyz);
                if (distanceToPointLight < lights[i].lightRange.x && pointLightIntesity > 0.0f)  //if there is light hitting surfice 
                {
                    float paraM = 1.0 / (lights[i].lightRange.x * lights[i].lightRange.x * 0.01);
                    float radius = sqrt(1.0 / (paraM * 0.01));
                    float att = clamp(1.0 - (lights[i].fallofFactor * (distanceToPointLight * distanceToPointLight)) / (radius * radius), 0.0, 1.0);

                    //float attenuation = 
                    //1.f + 0.01/*lights[i].fallofFactor */* (distanceToPointLight / lights[i].lightRange) * (distanceToPointLight / lights[i].lightRange); //+1 to avoid negative and zero something results that may give unWanted results

                    diffusePoint += pointLightIntesity * lights[i].lightColor.xyz * att /* / attenuation*/; //add together light,most light in line with lightsource    
                    saturate(diffusePoint);

                    pointReflection = normalize(2 * pointLightIntesity * trueNormal - pixelToPointLightVector.xyz); //
                    pointSpecular = lights[i].lightColor.rgb;
                    pointSpecular *= pow(saturate(dot(pointReflection * materialSpecular.xyz, input.base.camToWorldObject.xyz)), materialShininess.x); //pow=raised to
                    specular += pointSpecular;
                }
            }
            else if (lights[i].type == 2)
            {
                //Spotlight
                float3 pixelToSpotLightVector = normalize(lights[i].lightPosition.xyz - input.base.world.xyz); //L.. point in scene to spotLightsource 
                float spotLightIntesity = saturate(dot(trueNormal, pixelToSpotLightVector.xyz)); // dot(normal,lightvector) ger vinkeln emellan vector och normal-spotLight
                float distanceToSpotLight = length(input.base.world.xyz - lights[i].lightPosition.xyz);

                if (distanceToSpotLight < lights[i].lightRange)
                {
                    //from cone to pixel
                    float3 spotToPixelVector = -pixelToSpotLightVector;
                     //Relative angle 1.0 - -1.0
                    float relativeAngle = dot(spotToPixelVector, lights[i].lightDirection.xyz);
                    //Angle range 1.0 - -1.0 
                    float allowedAngleCone = cos(lights[i].lightCone * 0.5f);
                    float allowedAngleInnerCone = cos(lights[i].lightCone / 2 /*3.1415f * 20 / 180.0f*/ * 0.5f);

                    float diffrence = allowedAngleInnerCone - allowedAngleCone;
                    float intensity = clamp((relativeAngle - allowedAngleCone) / diffrence, 0.0, 1.0);
                    
                    float attenuation =
                        1.f + lights[i].fallofFactor * (distanceToSpotLight / lights[i].lightRange) * (distanceToSpotLight / lights[i].lightRange);
                        
                    diffuseSpot = (intensity * spotLightIntesity) * lights[i].lightColor.xyz / attenuation;
                    
                    spotReflection = normalize(2 * intensity * trueNormal - pixelToSpotLightVector.xyz);
                    spotSpecular = lights[i].lightColor.rgb;
                    spotSpecular *= pow(saturate(dot(spotReflection.xyz * materialSpecular.xyz, input.base.camToWorldObject.xyz)), materialShininess.x);

                    //if (relativeAngle > allowedAngleCone)
                    //{
                    //    float attenuation =
                    //    1.f + lights[i].fallofFactor * (distanceToSpotLight / lights[i].lightRange) * (distanceToSpotLight / lights[i].lightRange);
                    //    diffuseSpot += spotLightIntesity * lights[i].lightColor.xyz / attenuation; // * saturate(1 - distanceToSpotLight / spotLightRange); //add on light/material 
                    //    saturate(diffuseSpot);
            
                    //    spotReflection = normalize(2 * spotLightIntesity * trueNormal - pixelToSpotLightVector.xyz);
                    //    spotSpecular = lights[i].lightColor.rgb;
                    //    spotSpecular *= pow(saturate(dot(spotReflection.xyz * materialSpecular.xyz, input.base.camToWorldObject.xyz)), materialShininess.x);
                
                    //    specular += spotSpecular;
                    //}
                }
            }
        }
    }
    
    addOnColor = saturate((addOnColor + diffuse + diffuseDir + diffusePoint + diffuseSpot) * image.xyz); //Add ambient, diffuse and specular lights
    addOnColor = saturate(addOnColor + pointSpecular + spotSpecular + dirSpecular); //not multiply -to put on top and not affect color of image

    addOnColor = (addOnColor * colorMultiplier.rgb) + colorAdditive.rgb; //ColourHue Added

    //#define GAMMA_CORRECTION 1.25f //no longer used
    return pow(float4(abs(addOnColor).rgb, image.a), gammaCorrection);
    //return float4(addOnColor, 1);
    
	//return diffuseTex.Sample(WrapSampler, input.uv)/*.xyzw*/;
}
   