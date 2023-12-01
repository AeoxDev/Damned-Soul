#include "../Headers/VFX_Patterns.hlsli" // NOTE: Constant Buffer, sampler and data resides in header.
Texture2D flipBookTex : register(t6);

struct GS_OUT
{
    float4 position : SV_POSITION;
    float4 rgb : RGB;
    float2 uv : UV;
    float time : TIME;
    int patterns : PATTERNS;
    int VFXpatterns : VFXPATTERNS;
};

inline float4 AniRow(in GS_OUT input, in float animaVertStart, in bool singleRow, in float animaSpeed = 8); ////4x4 sections, First Row and Second Row
inline float4 AniFullSheet(in GS_OUT input); ////4x4 sections, Third Row and Forth Row


float4 main(GS_OUT input) : SV_TARGET
{
    int pattern = input.patterns;
    int vfxPattern = input.VFXpatterns;
    
    float4 image;
    
    // ---------- START OF FLIPBOOKING --------- //

    
    // Check if this particle used the "NO MOVEMENT"
    if (pattern != 14)
    {
            //Animation
            // 0 = SMOKE        // 1 = ARCH     // 2 = EXPLOSION    // 3 = FLAMETHROWER
            // 4 = IMPLOSION    // 5 = RAIN     // 6 = SINUS        // 7 = LIGHTNING
            // 8 = CIRCLE_FIELD // 9 = FIRE     // 10 = PULSE       // 11 = ICETHROWER
            // 12 = BOILING     // 13 = SPARK
    
            //Pattern can use animations as exampels seen below.
            //Examples
            //image = AniRow(input, 0.0f, true);    // Single row animation:: Top row= 0.0, SecondRow = 0.25 ,Third = 0.50 or bottom Row = 0.75
            //image = AniRow(input, 0.0f, false);   // DubbleRow animation:: Top rows= 0.0, bottom Rows = 0.5
            ///////// AniRow(input, 0.0f, false, specify animation speed- defalt is 8);
            //image = AniFullSheet(input);
            // AniRow(input, 0.0f, false, specify animation speed- defalt is 8);'
     
        
        image = flipBookTex.Sample(WrapSampler, float2(input.uv.x / 4, input.uv.y / 4));
        if (pattern == 0)
        {
            image = AniRow(input, 0.0f, false); /*AniFullSheet(input);*/
        }
        else if (pattern == 9) //4x4 sections, Top Row and Second Row A&B//pattern = 9 (FIRE)
        {
            image = AniRow(input, 0.0f, false); /*AniFullSheet(input);*/
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
    }
     // ---------- END OF FLIPBOOKING --------- //
    
    // *********** START OF VFX *********** //
    else
    {
        float4 backBuffer = SampleBackbuffer(input.position, screenResolution_in, backbufferTexture_in, WrapSampler);
        
        
        switch (vfxPattern)
        {
            case 0: // FLAME
                image = VFXFire(backBuffer, input.time, input.uv);
                break;
            case 1: // BLUE_FLAME
                image = VFXFire(backBuffer, input.time, input.uv, 2.0f, float3(0.0f, 0.25f, 1.0f));
                break;
            case 2: // SWORD
                image = VFXSwordSlash(backBuffer, input.time, input.uv);
                break;
            case 3: // ACID
                image = VFXAcidSpit(backBuffer, input.time, input.uv);
                break;
            case 4: // ACIDGROUND
                image = VFXAcidGround(backBuffer, input.time, input.uv);
                break;
            case 5: // FIREBALL
                image = VFXFireBall(backBuffer, input.time, input.uv);
                break;
            case 6: //FIREBALL_EMPOWERED
                image = VFXFireBallEmpowered(backBuffer, input.time, input.uv);
                break;
            case 7: // SPAWN_BOSS
                image = VFXSpawnCrystals(backBuffer, input.time, input.uv, 222.25f);
                // NOTE: IF YOU WANT TO MAKE IT GO FASTER, INCREASTE THE SPEED VALUE ( 0.25f )
                // FURTHER: SIZE UP THE PARTICLE, DONT LET IT BE SMOL ( Pliz)
                // IMPORTANT: MATCH THE PARTICLE LIFETIME OR THE ANIMATION WILL BE REPEATING INFINITELY.
                break;
            case 8: // SPAWN_IMP
                image = VFXSpawnImp(backBuffer, input.time, input.uv, 2.0f);
                // NOTE: IF YOU WANT TO MAKE IT GO FASTER, INCREASTE THE SPEED VALUE ( 2.0f )
                // FURTHER: SIZE UP THE PARTICLE, DONT LET IT BE SMOL ( Pliz)
                // IMPORTANT: MATCH THE PARTICLE LIFETIME OR THE ANIMATION WILL BE REPEATING INFINITELY.
                break;
            case 9: // SPAWN_IMP_EMPOWERED
                image = VFXSpawnEmpoweredImp(backBuffer, input.time, input.uv, 2.0f);
                // NOTE: IF YOU WANT TO MAKE IT GO FASTER, INCREASTE THE SPEED VALUE ( 2.0f )
                // FURTHER: SIZE UP THE PARTICLE, DONT LET IT BE SMOL ( Pliz)
                // IMPORTANT: MATCH THE PARTICLE LIFETIME OR THE ANIMATION WILL BE REPEATING INFINITELY.
                break;
            case 10: // PORTAL
                image = VFXPortal(backBuffer, input.time, input.uv);
                break;
        };
    }
    // *********** END OF VFX *********** //

    return image;
    //return float4(image.rgb, image.a);
}

float4 AniRow(in GS_OUT input, in float animaVertStart, in bool singleRow, in float animaSpeed) //4x4 sections, First Row and Second Row
{
    float framesPerSecond = animaSpeed;
    float sampelSize = 1 / framesPerSecond; //-sets counter depending on 8 parts of one second. = 1/8 = 0.125
    
    float devideVarible = framesPerSecond / 8;
    
    float4 image;
    int counter = 1;
    float time = 0.0f;
    time = (int) input.time;

    float rowMovement = 0;
    bool singleRowAni = singleRow;

    ///**Counter**/// -sets counter depending on 8 parts of one second.
    if (input.time >= (time + sampelSize /*0.125*/) && input.time < (time + (sampelSize * devideVarible) * 2) /*0.250*/)
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
    else if (input.time >= (time + (sampelSize * devideVarible) * 7) /*0.875*/ && input.time < (time + 0.99))
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
        image = flipBookTex.Sample(WrapSampler, float2(rowMovement + input.uv.x / 4, animaVertStart + input.uv.y / 4));
    }
 
    return image;
}

float4 AniFullSheet(in GS_OUT input)    //4x4 sections, Third Row and Forth Row
{
    float4 image;
    float time = 0.0f;
    float animaVertStart = 0.0f;
    time = (int) input.time;

    if (time % 2 == 0)
    {
        image = AniRow(input, animaVertStart, false, 8.0f);
    }
    else
    {
        image = AniRow(input, animaVertStart + 0.5, false, 8.0f);
    }

    return image;
}