Texture2D diffuseTexture_in : register(t0);
sampler diffuseSampler_in : register(s0);

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV_COORDINATES;
};

cbuffer pannerInput: register(b0) {
    float time_in; // Time in seconds. Base this on lifetime alive from particle system.
    float panSpeed_in; // How fast do you want the panning to be. A multiplier.
    float2 offsetXY_in; // Offset of the uv coordinates in x ( u ) and y ( v ). Clamped between 1 and -1 since its illogial to do other ones.
};

float4 PixelShaderOutput(GS_OUT gs_in)
{
    float2 uvCoordinates = float2(((gs_in.uv.x + clamp(offsetXY_in.x, -1.0f, 1.0f)) * (panSpeed_in * time_in)),
                                 ((gs_in.uv.y + clamp(offsetXY_in.y, -1.0f, 1.0f)) * (panSpeed_in * time_in)));
    
    float4 diffuseTexture = diffuseTexture_in.Sample(diffuseSampler_in, uvCoordinates);
    return diffuseTexture_in;
}

