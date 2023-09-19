struct GS_OUT
{
    float4 position : SV_POSITION;
    float3 velocity : VELOCITY;
    float4 rbg : RBG;
};

float4 main(GS_OUT input) : SV_TARGET
{
    return input.rbg;
}