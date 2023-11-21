RWTexture2D<unorm float4> outputGlowData : register(u0);
RWTexture2D<unorm float4> inputGlowData : register(u1);
RWTexture2D<unorm float4> backbuffer : register(u2);

Texture2D<unorm float4> depthSRV : register(t0);

// Not necessary, unless future information is required.
//cbuffer BlurInfo : register(b0)
//{
//    int instance; // The current number of repetitions (i)
//}

static const float e = 2.71828f;
static const float pi = 3.14159f;

float Gaussian(int x, int y, float sigma)
{
    // Blur factor, higher sigma means pixels are weighted more equally
    // Lower sigma means distance has a quicker drop-off rate
    //float sigma = 1.f;
    
    float x2 = pow(x, 2); // x^2
    float y2 = pow(y, 2); // y^2
    float o2 = pow(sigma, 2); // sigma^2
    float V2pio2 = sqrt(2 * pi * o2); // V(2 * pi * o2)
    
    return (1 / V2pio2) * pow(e, -((x2 + y2) / (2 * o2)));
}

[numthreads(32, 32, 1)]
void main( uint3 threadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{
    int2 index = int2(groupID.x * 32 + threadID.x, groupID.y * 32 + threadID.y);
       
    if (index.x > 1600 || index.y > 900)
    {
        return;
    }
    
    // Don't draw glow on top of UI.
    //if (depthSRV[index].a == 0)
    //{
    //    return;
    //}
    
    float total = 0;
    float4 output = float4(0, 0, 0, 0);
    
    // Calculate glow falloff.
    #define WIDTH_HEIGHT (10)
    #define SIGMA (5.f)
    for (int y = max(index.y - WIDTH_HEIGHT, 0); y < min(index.y + WIDTH_HEIGHT, 900); ++y)
    {
        for (int x = max(index.x - WIDTH_HEIGHT, 0); x < min(index.x + WIDTH_HEIGHT, 1600); ++x)
        {
            int2 h8t = int2(x, y);
            float temp = Gaussian(index.x - x, index.y - y, SIGMA);
            total += temp;
            output += /*float4(*/inputGlowData[h8t]/*.rgb, 1.0f)*/ * temp; // How to calculate alpha correctly
        }
    }
    // TODO: Figure out how to add everything together and make it look good!
    //  Add blur "on top of" existing glow texture.
    float3 tmp = output.rgb/total + inputGlowData[index].rgb * inputGlowData[index].a;
    float4 glow = float4(tmp, output.a/* / total*/) /*outputGlowData[index] + */ /*inputGlowData[index]*/ /*float4(tmp, inputGlowData[index].a) + (output / total)*/;
    float4 bb_rgba = backbuffer[index];
    
    // Add glow color on backbuffer, using alpha as a factor.
    float fac_a = glow.a;
    float res_r = bb_rgba.r + glow.r * fac_a; /*(bb_rgba.r * (1 - fac_a)) + (glow.r * fac_a); */ // TODO: Try tweaking. Would multiply work better?
    float res_g = bb_rgba.g + glow.g * fac_a; /*(bb_rgba.g * (1 - fac_a)) + (glow.g * fac_a);*/
    float res_b = bb_rgba.b + glow.b * fac_a; /*(bb_rgba.b * (1 - fac_a)) + (glow.b * fac_a);*/
    
    backbuffer[index] = float4(res_r, res_g, res_b, bb_rgba.a);

}