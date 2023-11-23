Texture2D flipBookTex : register(t2);
SamplerState WrapSampler : register(s2);

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV;
    float time : TIME;
    int patterns : PATTERNS;
};

float4 AniRowABCD(float counter, GS_OUT input, int time)////4x4 sections, First Row and Second Row
{
    float4 image;
   
    if (time % 2 == 0)
    {
        if (counter == 1)
        {
            image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.5 + input.uv.y / 4));
        }
        else if (counter == 2)
        {
            image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.5 + input.uv.y / 4));
        }
        else if (counter == 3)
        {
            image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.5 + input.uv.y / 4));
        }
        else if (counter == 4)
        {
            image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.5 + input.uv.y / 4));
        }
        else if (counter == 5)
        {
            image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.75 + input.uv.y / 4));
        }
        else if (counter == 6)
        {
            image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.75 + input.uv.y / 4));
        }
        else if (counter == 7)
        {
            image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.75 + input.uv.y / 4));
        }
        else if (counter == 8)
        {
            image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.75 + input.uv.y / 4)); 
            
        }   
    }
    else
    {
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
        }
        else if (counter == 5)
        {
            image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.25 + input.uv.y / 4));
        }
        else if (counter == 6)
        {
            image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.25 + input.uv.y / 4));
        }
        else if (counter == 7)
        {
            image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.25 + input.uv.y / 4));
        }
        else if (counter == 8)
        {
            image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.25 + input.uv.y / 4));
        }
    }
        
    return image;
}

float4 AniRowAB(float counter, GS_OUT input)////4x4 sections, First Row and Second Row
{
    float4 image; 
    //image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, input.uv.y / 4));
    
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
    }
    else if (counter == 5)
    {
        image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.25 + input.uv.y / 4));
    }
    else if (counter == 6)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.25 + input.uv.y / 4));
    }
    else if (counter == 7)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.25 + input.uv.y / 4));
    }
    else if (counter == 8)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.25 + input.uv.y / 4));
    }
    
    //float2 newUV = float2(input.uv.x / 4 + 1 / counter);
    
    return image;
}

float4 AniRowCD(float counter, GS_OUT input)////4x4 sections, Third Row and Forth Row
{
    float4 image;
    
    if (counter == 1)
    {
        image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.5 + input.uv.y / 4));
    }
    else if (counter == 2)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.5 + input.uv.y / 4));
    }
    else if (counter == 3)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.5 + input.uv.y / 4));
    }
    else if (counter == 4)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.5 + input.uv.y / 4));
    }
    else if (counter == 5)
    {
        image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.75 + input.uv.y / 4));
    }
    else if (counter == 6)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.75 + input.uv.y / 4));
    }
    else if (counter == 7)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.75 + input.uv.y / 4));
    }
    else if (counter == 8)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.75 + input.uv.y / 4));
    }
    
    return image;

}

float4 AniRowA(float counter, GS_OUT input)////4x4 sections, First Row 
{
    float4 image;
    
    if (counter == 1 || counter == 5)
    {
        image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, input.uv.y / 4));
    }
    else if (counter == 2 || counter == 6)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, input.uv.y / 4));
    }
    else if (counter == 3 || counter == 7)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, input.uv.y / 4));
    }
    else if (counter == 4 || counter == 8)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, input.uv.y / 4));
    }
    
    return image;

}

float4 AniRowB(float counter, GS_OUT input)////4x4 sections, Second Row 
{
    float4 image;
    
    if (counter == 1 || counter == 5)
    {
        image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.25 + input.uv.y / 4));
    }
    else if (counter == 2 || counter == 6)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.25 + input.uv.y / 4));
    }
    else if (counter == 3 || counter == 7)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.25 + input.uv.y / 4));
    }
    else if (counter == 4 || counter == 8)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.25 + input.uv.y / 4));
    }
    
    return image;
}

float4 AniRowC(float counter, GS_OUT input)////4x4 sections, Third Row 
{
    float4 image;
    
    if (counter == 1 || counter == 5)
    {
        image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.5 + input.uv.y / 4));
    }
    else if (counter == 2 || counter == 6)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.5 + input.uv.y / 4));
    }
    else if (counter == 3 || counter == 7)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.5 + input.uv.y / 4));
    }
    else if (counter == 4 || counter == 8)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.5 + input.uv.y / 4));
    }
    
    return image;
}

float4 AniRowD(float counter, GS_OUT input)////4x4 sections, Forth Row 
{
    float4 image;
    
    if (counter == 1 || counter == 5)
    {
        image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, 0.75 + input.uv.y / 4));
    }
    else if (counter == 2 || counter == 6)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.25 + input.uv.x / 4, 0.75 + input.uv.y / 4));
    }
    else if (counter == 3 || counter == 7)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.5 + input.uv.x / 4, 0.75 + input.uv.y / 4));
    }
    else if (counter == 4 || counter == 8)
    {
        image = flipBookTex.Sample(WrapSampler, float2(0.75 + input.uv.x / 4, 0.75 + input.uv.y / 4));
    }
    
    return image;
}

float4 main(GS_OUT input) : SV_TARGET
{
    int counter = 1;
    float time = 0.0f;
    int pattern = input.patterns;
    time = (int)input.time;
    
    ////if (input.time >= (time + 0.25) && input.time < (time + 0.50)) //To slow
    ////{
    ////    counter = 1;
    ////}
    ////else if (input.time >= (time + 0.50) && input.time < (time + 0.75))
    ////{
    ////    counter = 2;
    ////}
    ////else if (input.time >= (time + 0.75) && input.time < (time + 0.99))
    ////{
    ////    counter = 3;
    ////}
    ////else if (input.time >= (time + 0.99) || input.time < (time + 0.249))
    ////{
    ////    counter = 4;
    ////}
    
    if(input.time >= (time + 0.125) && input.time < (time + 0.250))
    {
        counter = 1;
    }
    else if (input.time >= (time + 0.250) && input.time < (time + 0.375))
    {
        counter = 2;
    }
    else if (input.time >= (time + 0.375) && input.time < (time + 0.50))
    {
        counter = 3;
    }
    else if (input.time >= (time + 0.50) && input.time < (time + 0.625))
    {
        counter = 4;
    }
    else if (input.time >= (time + 0.625) && input.time < (time + 0.75))
    {
        counter = 5;
    }
    else if (input.time >= (time + 0.75) && input.time < (time + 0.875))
    {
        counter = 6;
    }
    else if (input.time >= (time + 0.875) && input.time < (time + 0.99))
    {
        counter = 7;
    }
    else if (input.time >= (time + 0.99) || input.time < (time + 0.125))
    {
        counter = 8;
    }
  
    
    //Animation
    // 0 = SMOKE// 1 = ARCH// 2 = EXPLOSION// 3 = FLAMETHROWER// 4 = IMPLOSION// 5 = RAIN// 6 = SINUS// 7 = LIGHTNING
    float4 image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, input.uv.y / 4));
    
    //Pattern can use animation on Row A,B,C,D or A&B or C&D
    if (pattern == 8) //4x4 sections, Top Row and Second Row A&B//patterns = //test 0(SMOKE)
    {
        image = AniRowAB(counter, input);
        //image = AniRowCD(counter, input);

    }
    else if (pattern == 0) //4x4 sections, Third and Forth Row C&D//patterns = test 1(ARCH)
    {
        //image = AniRowCD(counter, input);
        //image = AniRowABCD(counter, input, (int) time);
    }
    else
    {
        image = flipBookTex.Sample(WrapSampler, input.uv);
    }
   
    clip(image.a - 0.1f);
    
    if (image.r == 1.0f && image.g == 1.0f && image.b == 1.0f)
    {
        image = input.rgb;
    }
    
    //return input.rgb;
    return float4(image.rgb, image.a);
}