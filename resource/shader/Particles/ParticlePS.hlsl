Texture2D flipBookTex : register(t2);
SamplerState WrapSampler : register(s2);

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV;
    float time : TIME;
};

float4 main(GS_OUT input) : SV_TARGET
{
    int counter = 0;
    float differ = 0.0f;
    float time = 0.0f;
    
    //float4 image = (0.0f, 0.0f, 0.0f, 0.0f);
    //float4 image = flipBookTex.Sample(WrapSampler, input.uv); //texturImage 
    
    //pos = timeInSeconds;
    counter = input.time;
    
    if ((int)differ != (int)time)
    {
        counter++;
        
        //image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, input.uv.y / 4));
    }
    
    float4 image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, input.uv.y / 4));
    //Animation
    
    //4x4 sections, Top Row
    if (counter == 1)
    {
        image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, input.uv.y / 4));
    }
    else if (counter == 2)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, input.uv.y / 4));
    }
    else if (counter == 3)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, input.uv.y / 4));
    }
    else if (counter == 4)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, input.uv.y / 4));
        counter = 0;
    }
    //X=1,Y=1
    //float4 image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, input.uv.y / 4));
    //X=2,Y=1
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, input.uv.y / 4));
    //X=3,Y=1
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, input.uv.y / 4)); 
    //X=4,Y=1
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, input.uv.y / 4)); 
    
    //4x4 sections, Second Row
    
    //X=1,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.25 + input.uv.y / 4));
    //X=2,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.25 +  input.uv.y / 4));
    //X=3,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.25 +  input.uv.y / 4));
    //X=4,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.25 + input.uv.y / 4)); 
    
    //4x4 sections, Third Row
    
    //X=1,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.5 + input.uv.y / 4));
    //X=2,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.5 +  input.uv.y / 4));
    //X=3,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.5 +  input.uv.y / 4));
    //X=4,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.5 + input.uv.y / 4));
    
    //4x4 sections, Forth Row
    
    //X=1,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.75 + input.uv.y / 4));
    //X=2,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.75 +  input.uv.y / 4));
    //X=3,Y=2
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.75 +  input.uv.y / 4));
    //X=4,Y=2
   
    //float4 image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.75 + input.uv.y / 4));
    
    clip(image.a - 0.1f);
    
    if (image.r == 1.0f && image.g == 1.0f && image.b == 1.0f)
    {
        image = input.rgb;
    }
    
    //return input.rgb;
    return float4(image.rgb, image.a);
}