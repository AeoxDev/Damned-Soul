
struct PS_IN
{
    float4 position : SV_POSITION; //world, view, projection - multiplyed //Position on screen
    float4 normal : WNORMAL; // world - multiplyed
    float2 uv : UV;
    float4 camToWorldObject : CAM; // normalized 
    float4 world : WORLD;
    float2 depth : DEPTH;
};

float main(PS_IN input) : SV_TARGET
{
    return input.depth.x / input.depth.y;
}