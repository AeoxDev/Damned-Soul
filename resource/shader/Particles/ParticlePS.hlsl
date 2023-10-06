struct GS_OUT
{
    float4 worldPosition : SV_POSITION;
    float4 rbg : RBG;
};

float4 main(GS_OUT input) : SV_TARGET
{
    return input.rbg;
}