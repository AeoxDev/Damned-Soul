cbuffer GIConstantBufferData : register(b4)
{
	//Contains what is needed for the pixel shader to know what it should be doing
    float heightThreshold; //The height in y to consider as stage or not
    float idValue; //1 is stage, 2+ are static hazards, this is not used when reading from textures
    float padding;
    bool isTexture; //When hazard, if texture, read from texture instead.
};

SamplerState WrapSampler : register(s0);

Texture2D hazardTexture : register(t0);

struct PS_IN
{
    float4 svPosition : SV_POSITION;
    float4 position : POSITION;
    float2 uv : UV;
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
    if (isTexture)
    {
        float4 color =  hazardTexture.Sample(WrapSampler, input.uv);
        if (color.r > 0.5f)
        {
            result = idValue;
        }
        else
        {
            clip(-1.0f);
        }
    }
    else if (input.position.z > -10.0f)
    {
        result = idValue;
    }
    return uint4(result, 0, 0, 0);
}