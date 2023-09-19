
struct PS_IN
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    uint material : MATERIAL;
    float2 uv : UV;
};

//Make use of a pre-existing pixel shader constant buffer for GI data in order to reduce memory usage

float4 main(PS_IN input) : SV_TARGET
{
    //Using an orthographic camera in the vertex stage:
    //Look at model and see whatever is above pre-determined value
    //Whatever is above the line is stage (1.0f), otherwise it is not stage 0.0f
    
    //For the hazards, Either read a texture to place 2d static hazards
    //Or use the same method from the stage and look at 3d objects
    //This time, however, the values will be either clip or 2+, depending on the hazard
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}