struct PixelShaderInput
{
    float4 position : SV_POSITION;
};

struct PixelShaderOutput
{
    float4 swapChain : SV_Target;
};

PixelShaderOutput main(PixelShaderInput input)
{
    PixelShaderOutput output;

    output.swapChain = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return output;
}