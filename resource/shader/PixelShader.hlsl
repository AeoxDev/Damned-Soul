Texture2D diffuseTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D emissionTex : register(t2);

SamplerState WrapSampler : register(s0);

cbuffer PS_DIRECTIONAL_LIGHT : register(b2)
{
    float4 dirLightColor;
    float4 dirLightDirection; //normalize
};

//cbuffer PS_POINT_LIGHT : register(b2)
//{
//    float4 pointLightColor;
//    float4 pointLightPosition;
//    float4 pointLightRange;
//};

//cbuffer PS_SPOT_LIGHT : register(b3)
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

struct PS_IN
{
    float4 position : SV_POSITION; //world, view, projection - multiplyed
    float4 normal : WNORMAL; // world - multiplyed
    float2 uv : UV;
    float4 camToWorldObject : CAM; // normalized 
};

//struct PS_OUT
//{
//    float4 position : SV_Target0;
//    float4 normal : SV_Target1;
//    float4 SamplerColor : SV_Target2;

//};

float4 main(PS_IN input) : SV_TARGET
{
    float4 materialAmbient = { 1.0f, 1.0f, 1.0f, 1.0f };
    float4 materialDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    float4 materialSpecular = { 10.0f, 10.0f, 10.0f, 1.0f };
    float4 materialShininess = { 50.10f, 0.0f, 0.0f, 0.0f };

    float4 image = diffuseTex.Sample(WrapSampler, input.uv); //texturImage
    
    float4 addOnColor = materialAmbient; //lighting-effects to apply to texture
    float4 diffuse = materialDiffuse;

    ///xxxx DirectionalLight xxxx///
    float4 dirReflection = { 0.0f, 0.0f, 0.0f, 0.0f };
    float4 dirSpecular = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    float4 invertedDirLightDirection = -dirLightDirection;
    float dirLightIntesity = saturate(dot(input.normal, invertedDirLightDirection));
    
    if (dirLightIntesity > 0.0f)
    {
       //diffuse??
        
        addOnColor += (dirLightColor * dirLightIntesity);
        saturate(addOnColor);

        dirReflection = normalize(2 * dirLightIntesity * input.normal - invertedDirLightDirection);
        dirSpecular = dirLightColor;
        dirSpecular *= pow(saturate(dot(dirReflection * materialSpecular, input.camToWorldObject)), materialShininess.x);
    }
    
    ///xxxx PointLight xxxx///
    
    //float4 pointReflection = { 0.0f, 0.0f, 0.0f, 0.0f };
    //float4 pointSpecular = { 0.0f, 0.0f, 0.0f, 0.0f };
    //float4 pixelToPointLightVector = normalize(pointLightPosition - input.position); //L.. point in scene to pointLightsource 
    //float pointLightIntesity = saturate(dot(input.normal, pixelToPointLightVector)); // dot(normal,lightvector) ger vinkeln emellan vector och normal -pointLight
    //float distanceToPointLight = length(input.position - pointLightPosition);
    
    //if (distanceToPointLight < pointLightRange.x && pointLightIntesity > 0.0f)  //if there is light hitting surfice //vinkeln är större än noll 
    //{
    //    //addOnColor *= pointLightAmbient;
                
    //         //basicFallOff//
    //    float lightFadeArea = saturate(max(0, distanceToPointLight - pointLightRange.x)); //fadeArea <0 inside lightRange
    //    float attenuation = 1.0f + lightFadeArea; //+1 för att undvika -resultat och noll komma något resultat som ger oönskade resultat 
   
    //         //basicFallOff controlled//
    //    //float constant_func = 1.0f;
    //    //float linear_func = 1.0f;    //fade more set at 1 fade less att 100
    //    //float quadratic_func = 1.0f; //fade more set at 1 fade less att 100
    //    //float attenuationTwo = constant_func + linear_func * lightFadeArea + quadratic_func * lightFadeArea * lightFadeArea; 
    
    //    diffuse *= pointLightIntesity * pointLightColor / attenuation; //lägg ihop ljus .. störst lightintensity när den träffar i linje med ljuskällan  
    //    ////
    //    addOnColor += (diffuse); //add on light/material 
    //    saturate(addOnColor);
            
    //    pointReflection = normalize(2 * pointLightIntesity * input.normal - pixelToPointLightVector); //
    //    pointSpecular = pointLightColor;
    //    pointSpecular *= pow(saturate(dot(pointReflection * materialSpecular, input.camToWorldObject)), materialShininess.x); //pow-uphöjt 
            
    //}
    
    ///xxxx SpotLight xxxx///
    
    //float4 spotReflection = { 0.0f, 0.0f, 0.0f, 0.0f };
    //float4 spotSpecular = { 0.0f, 0.0f, 0.0f, 0.0f };
    //float4 pixelToSpotLightVector = normalize(spotLightPosition - input.position); //L.. point in scene to spotLightsource 
    //float spotLightIntesity = saturate(dot(input.normal, pixelToSpotLightVector)); // dot(normal,lightvector) ger vinkeln emellan vector och normal-spotLight
    //float distanceToSpotLight = length(input.position - spotLightPosition);
    
    //if (distanceToSpotLight < spotLightRange.x)
    //{
    //    float minAngle = cos(spotLightCone.x) + 1.0f; // inverted cos cone
    //    float maxAngle = (minAngle + 1.0f) / 2.0f; //cos angle + 180 degrees than half of this
    //    float angle = dot(pixelToSpotLightVector, -spotLightDirection);
    //    float coneIntensity = smoothstep(minAngle, maxAngle, angle);
        
    //    spotLightIntesity *= coneIntensity;
        
    //    if (spotLightIntesity > 0.0f)
    //    {
    //        diffuse += saturate(spotLightIntesity * spotLightColor) * saturate(1 - distanceToSpotLight / spotLightRange); //add on light/material 
                
    //        spotReflection = normalize(2 * spotLightIntesity * input.normal - pixelToSpotLightVector);
    //        spotSpecular = spotLightColor;
    //        spotSpecular *= pow(saturate(dot(spotReflection * materialSpecular, input.camToWorldObject)), materialShininess.x);
    //    }
    //}
    
    addOnColor = saturate(addOnColor * image); //multiply light-calculations with texture
    
    addOnColor = saturate(addOnColor /*+ pointSpecular + spotSpecular */+ dirSpecular); //not multiply to put on top and not affect color of image
    
    return float4(addOnColor);
    
	//return diffuseTex.Sample(WrapSampler, input.uv)/*.xyzw*/;
}
   