Texture2D diffuseTexture_in : register(t0);
sampler diffuseSampler_in : register(s0);

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV_COORDINATES;
    float time : TIME;
};

cbuffer pannerInput: register(b0) {
    float panSpeed_in; // How fast do you want the panning to be. A multiplier.
    float2 offsetXY_in; // Offset of the uv coordinates in x ( u ) and y ( v ). Clamped between 1 and -1 since its illogial to do other ones.
};

float4 main(GS_OUT gs_in) : SV_Target
{
    float2 uvCoordinates = float2(
                                  ( (gs_in.uv.x + offsetXY_in.x) * (panSpeed_in * gs_in.time)  ),
                                  ( (gs_in.uv.y + offsetXY_in.y) * (panSpeed_in * gs_in.time) )
                                  );
    
    float4 diffuseTexture = diffuseTexture_in.Sample(diffuseSampler_in, uvCoordinates);
    return diffuseTexture;
}

