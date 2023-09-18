
struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 screenPos : SCREEN_POS;
};

VS_OUT main(float3 position : POSITION)
{
    VS_OUT output;
    output.screenPos = float4(position, 1.0f);
    output.position = float4(position, 1.0f);
    
    return output;
}