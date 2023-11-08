struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
};

float4 main(GS_OUT input) : SV_TARGET
{
    return input.rgb;
}