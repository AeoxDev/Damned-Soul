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

float4 AniRow(GS_OUT input, float animaVertStart, bool singleRow, float animaSpeed = 8)////4x4 sections, First Row and Second Row
{
    float framesPerSecond = animaSpeed;
    float sampelSize = 1 / framesPerSecond; //-sets counter depending on 8 parts of one second. = 1/8 = 0.125
    
    float devideVarible = framesPerSecond / 8;
    
    float4 image;
    int counter = 1;
    float time = 0.0f;
    time = (int)input.time;

    float rowMovement = 0;
    bool singleRowAni = singleRow;

    ///**Counter**/// -sets counter depending on 8 parts of one second.
    if (input.time >= (time + sampelSize /*0.125*/)  && input.time < (time + (sampelSize * devideVarible) * 2) /*0.250*/)
    {
        counter = 1;
        rowMovement = 0.0f;

    }
    else if (input.time >= (time + (sampelSize * 2) /*0.250*/) && input.time < (time + (sampelSize * devideVarible) * 3) /*0.375*/)
    {
        counter = 2;
        rowMovement = 0.25f;

    }
    else if (input.time >= (time + (sampelSize * 3) /*0.375*/) && input.time < (time + (sampelSize * devideVarible) * 4) /*0.50*/)
    {
        counter = 3;
        rowMovement = 0.50f;

    }
    else if (input.time >= (time + (sampelSize * 4) /*0.50*/) && input.time < (time + (sampelSize * devideVarible) * 5) /*0.625*/)
    {
        counter = 4;
        rowMovement = 0.75f;

    }
    else if (input.time >= (time + (sampelSize * 5) /*0.625*/) && input.time < (time + (sampelSize * devideVarible) * 6) /*0.75*/)
    {
        counter = 5;
        rowMovement = 0.0f;

    }
    else if (input.time >= (time + (sampelSize * 6) /*0.75*/) && input.time < (time + (sampelSize * devideVarible) * 7) /*0.875*/)
    {
        counter = 6;
        rowMovement = 0.25f;

    }
    else if (input.time >=(time + (sampelSize * devideVarible) * 7) /*0.875*/ && input.time < (time + 0.99))
    {
        counter = 7;
        rowMovement = 0.50f;

    }
    else if (input.time >= (time + 0.99) || input.time < (time + sampelSize /*0.125*/))
    {
        counter = 8;
        rowMovement = 0.75f;

    }

    if (counter >= 5 && !singleRowAni)
    {
        image = flipBookTex.Sample(WrapSampler, float2(rowMovement + input.uv.x / 4, (animaVertStart + 0.25) + input.uv.y / 4));
    }
    else
    {
        image = flipBookTex.Sample(WrapSampler, float2(rowMovement + input.uv.x / 4, animaVertStart+ input.uv.y / 4));
    }
 
    return image;
}

float4 AniFullSheet(GS_OUT input)////4x4 sections, Third Row and Forth Row
{
    float4 image;
    float time = 0.0f;
    float animaVertStart = 0.0f;
    time = (int)input.time;

    if (time % 2 == 0)
    {
        image = AniRow(input, animaVertStart, false);
    }
    else
    {
        image = AniRow(input, animaVertStart + 0.5, false);
    }

    return image;
}

float4 main(GS_OUT input) : SV_TARGET
{
    int pattern = input.patterns;

    //Animation
    // 0 = SMOKE// 1 = ARCH// 2 = EXPLOSION// 3 = FLAMETHROWER// 
    //4 = IMPLOSION// 5 = RAIN// 6 = SINUS// 7 = LIGHTNING
    //8= CIRCLE_FIELD// 9 =FIRE
    
    float4 image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, input.uv.y / 4));
    
    //Pattern can use animations as exampels seen below.
    //Examples
    //image = AniRow(input, 0.0f, true);    // Single row animation:: Top row= 0.0, SecondRow = 0.25 ,Third = 0.50 or bottom Row = 0.75
    //image = AniRow(input, 0.0f, false);   // DubbleRow animation:: Top rows= 0.0, bottom Rows = 0.5
    ///////// AniRow(input, 0.0f, false, specify animation speed- defalt is 8);
    //image = AniFullSheet(input);
    // AniRow(input, 0.0f, false, specify animation speed- defalt is 8);

    if (pattern == 9) //4x4 sections, Top Row and Second Row A&B//pattern = 9 (FIRE)
    {
        image = AniRow(input, 0.0f, false);/*AniFullSheet(input);*/
    }
    else if (pattern == 12) //4x4 sections, Third and Forth Row C&D//pattern = test 0(SMOKE)
    {
        image = AniRow(input, 0.50f, false);
    }
    else if (pattern == 13) //4x4 sections, Third and Forth Row C&D//pattern = test 0(SMOKE)
    {
        image = AniRow(input, 0.50f, false, 16);
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