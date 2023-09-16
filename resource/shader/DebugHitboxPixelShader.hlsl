struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct PixelShaderOutput
{
    float4 swapChain : SV_Target;
};

PixelShaderOutput main(PixelShaderInput input)
{
    PixelShaderOutput output;

    output.swapChain = input.color;
    return output;
}