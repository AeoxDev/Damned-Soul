
struct PS_IN
{
    float4 svPosition : SV_POSITION;
    float4 position : POSITION;
};

uint4 main(PS_IN input) : SV_TARGET
{
    //Using an orthographic camera in the vertex stage:
    //Look at model and see whatever is above pre-determined value
    //Whatever is above the line is stage (1.0f), otherwise it is not stage 0.0f
    
    //For the hazards, Either read a texture to place 2d static hazards
    //Or use the same method from the stage and look at 3d objects
    //This time, however, the values will be either clip or 2+, depending on the hazard
    uint result = 0;
    if (input.position.z > -10.0f)
    {
        result = 1;
    }
    return uint4(result, 0, 0, 0);
}