#include "VFX_Patterns.hlsli" // NOTE: Constant Buffer, sampler and data resides in header.

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV_COORDINATES;
    float time : TIME;
};

float4 main(GS_OUT gs_in) : SV_Target
{
    // NOTE: REQUIRED for Alpha
    float4 backBuffer = SampleBackbuffer(gs_in.position, screenResolution_in, backbufferTexture_in, diffuseSampler_in);
    
    //float4 vornoiTexture = vornoiTexture_in.Sample(diffuseSampler_in, UVPan(offsetXY_in, panSpeed_in * 1.2f, gs_in.time, gs_in.uv));
    //float4 gNoiseTexture = noiseTexture_in.Sample(diffuseSampler_in, UVPan(offsetXY_in, panSpeed_in, gs_in.time * 2.0f, gs_in.uv));
    //
    //float2 vornoiUVs = distortUV(0.25f, gs_in.uv, pow(vornoiTexture, 0.5f));
    //float2 noiseUVs = distortUV(0.08f, vornoiUVs, gNoiseTexture);
    
    //float4 vornoiColor = vornoiTexture_in.Sample(diffuseSampler_in, gs_in.uv * .4f);
    //float4 vornoiManip = vornoiTexture_in.Sample(diffuseSampler_in, UVPan(offsetXY_in, panSpeed_in * 0.25f, gs_in.time, 0.5f * noiseUVs));
    //    
    //float4 gradientMask = colorRampTexture_in.Sample(diffuseSampler_in, gs_in.uv);
    //
    //float4 flowTexture = pow((gradientMask.r * vornoiManip), 0.65f * gs_in.time);
    //float4 mask = shapeTexture_in.Sample(diffuseSampler_in, noiseUVs);

    return VFXFire(backBuffer, gs_in.time, gs_in.uv); //AlphaBlend(backBuffer, mask.r * gradientMask.r, float4(flowTexture.r * 1.0f* gs_in.time, flowTexture.b * 0.0f, flowTexture.g * 0.1f * gs_in.time, 1.0f));
   

}

