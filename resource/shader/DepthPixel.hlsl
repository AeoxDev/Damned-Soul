
struct VS_OUT
{
    float4 position : SV_POSITION; //world, view, projection - multiplied
    float4 normal : WNORMAL; //world - multiplied
    float2 uv : UV;
    float4 camToWorldObject : CAM; // normalized 
    float4 world : WORLD;
};



float4 main(VS_OUT input) : SV_TARGET
{
    return float4(input.world.x, input.world.y, input.world.z, input.position.w);
}