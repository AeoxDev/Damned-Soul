RWTexture2D<unorm float4> backbuffer : register(u0);

Texture2D<unorm float4> uiSRV : register(t0);
Texture2D<unorm float4> inputGlowData : register(t1);
Texture2D<unorm float4> outlineSRV : register(t2);

cbuffer GlowInfo : register(b0)     // HELP: Unsiure if this is input correctly. Gives strange results compared to "the normal".
{
    int windowWidth;
    int windowHeight;
}

static const float e = 2.71828f;
static const float pi = 3.14159f;

// Calculate blur factor. Higher sigma means pixels are weighted more equally, lower sigma means distance has a quicker drop-off rate.
float Gaussian(in int x, in int y, in float sigma)
{
    float x2 = pow(x, 2); // x^2
    float y2 = pow(y, 2); // y^2
    float o2 = pow(sigma, 2); // sigma^2
    float V2pio2 = sqrt(2 * pi * o2); // V(2 * pi * o2)
    
    return (1 / V2pio2) * pow(e, -((x2 + y2) / (2 * o2)));
}

float4 lerp4(float4 a, float4 b, float s)
{
    return float4(lerp(a.x, b.x, s), lerp(a.y, b.y, s), lerp(a.z, b.z, s), lerp(a.w, b.w, s));
}

[numthreads(32, 32, 1)]
void main( uint3 threadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{
    int2 index = int2(groupID.x * 32 + threadID.x, groupID.y * 32 + threadID.y);
       
    if (index.x > windowWidth || index.y > windowHeight)
    {
        return;
    }
    
    // Don't draw glow on top of UI.
    if (uiSRV[index].a != 0)
    {
        return;
    }
    
    float outlineTotal = 0;
    float4 outlineAdd = float4(0, 0, 0, 0);
    float4 baseOultine = outlineSRV[index];
    
    #define WIDTH_HEIGHT_OUTLINE (3)
    #define SIGMA_OUTLINE (7.0f)
    for (int y = max(index.y - WIDTH_HEIGHT_OUTLINE, 0); y < min(index.y + WIDTH_HEIGHT_OUTLINE, windowHeight); ++y)
    {
        for (int x = max(index.x - WIDTH_HEIGHT_OUTLINE, 0); x < min(index.x + WIDTH_HEIGHT_OUTLINE, windowWidth); ++x)
        {
            int2 h8t = int2(x, y);
            float temp = Gaussian(index.x - x, index.y - y, SIGMA_OUTLINE);
            outlineTotal += temp;
            outlineAdd += outlineSRV[h8t] * temp;
        }
    }
    outlineAdd = outlineAdd / outlineTotal;
    outlineAdd = outlineAdd * (1.f - baseOultine.a);
    //outlineAdd = lerp4(backbuffer[index], float4(pow(1.f - backbuffer[index].r, 2), pow(1.f - backbuffer[index].g, 2), 1.f - pow(1.f - backbuffer[index].b, 2), 1.f), outlineAdd.a);
    outlineAdd = lerp4(backbuffer[index], pow(backbuffer[index] - 0.15f, 3.f), outlineAdd.a);
    
    float total = 0;
    float4 output = float4(0, 0, 0, 0);
    // Calculate color blend and glow falloff based on pixel distance.
    #define WIDTH_HEIGHT_GLOW (10)
    #define SIGMA_GLOW (6.f)
    for (int y = max(index.y - WIDTH_HEIGHT_GLOW, 0); y < min(index.y + WIDTH_HEIGHT_GLOW, windowHeight); ++y)
    {
        for (int x = max(index.x - WIDTH_HEIGHT_GLOW, 0); x < min(index.x + WIDTH_HEIGHT_GLOW, windowWidth); ++x)
        {
            int2 h8t = int2(x, y);
            float temp = Gaussian(index.x - x, index.y - y, SIGMA_GLOW);
            total += temp;
            output += inputGlowData[h8t] * temp;
        }
    }
    
    //  Add blur "on top of" existing glow texture.
    float3 tmp = output.rgb/total + inputGlowData[index].rgb * inputGlowData[index].a;
    float4 glow = float4(tmp, output.a);
    float4 bb_rgba = outlineAdd; //backbuffer[index];
    
    // Add glow color on backbuffer, using alpha as a factor.
    float fac_a = glow.a;
    float res_r = bb_rgba.r + glow.r * fac_a; // TODO: Tweak.
    float res_g = bb_rgba.g + glow.g * fac_a;
    float res_b = bb_rgba.b + glow.b * fac_a;
    
    backbuffer[index] = float4(res_r, res_g, res_b, bb_rgba.a);

}