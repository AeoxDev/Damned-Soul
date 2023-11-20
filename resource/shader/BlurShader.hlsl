RWTexture2D<unorm float4> outputGlowData : register(u0);
RWTexture2D<unorm float4> inputGlowData : register(u1);
RWTexture2D<unorm float4> backbuffer : register(u2);


// Not necessary, unless future information is required.
cbuffer BlurInfo : register(b0)
{
    int instance; // The current number of repetitions (i)
}

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
    
    float total = 0;
    float4 output = float4(0, 0, 0, 0);
    
    #define WIDTH_HEIGHT (10)
    #define SIGMA (5.f)
    for (int y = max(index.y - WIDTH_HEIGHT, 0); y < min(index.y + WIDTH_HEIGHT, 900); ++y)
    {
        for (int x = max(index.x - WIDTH_HEIGHT, 0); x < min(index.x + WIDTH_HEIGHT, 1600); ++x)
        {
            int2 h8t = int2(x, y);
            float temp = Gaussian(index.x - x, index.y - y, SIGMA);
            total += temp;
            output += inputGlowData[h8t] * temp;
        }
    }
    
    outputGlowData[index] += inputGlowData[index] + (output / total);    // Add blur "on top of" existing texture.
    
    //float4 m_rgb = inputGlowData[index.xy].rgba;
    
    //int2 up = int2(index.x, index.y - 1);
    //int2 down = int2(index.x, index.y + 1);
    //int2 left = int2(index.x - 1, index.y);
    //int2 right = int2(index.x + 1, index.y);
    
    //float4 up_rgb;
    //float4 down_rgb;
    //float4 left_rgb;
    //float4 right_rgb;
    
    //int calc_pxl = 3;
    
    //if (up.y < 0)
    //{
    //    up_rgb = float4(0, 0, 0, 0);
    //}
    //else
    //{
    //    up_rgb = inputGlowData[up].rgba;
    //    calc_pxl++;
    //}
    //if (down.y > 900)
    //{
    //    down_rgb = float4(0, 0, 0, 0);
    //}
    //else
    //{
    //    down_rgb = inputGlowData[down].rgba;
    //    calc_pxl++;
    //}
    //if (left.x < 0)
    //{
    //    left_rgb = float4(0, 0, 0, 0);
    //}
    //else
    //{
    //    left_rgb = inputGlowData[left].rgba;
    //    calc_pxl++;
    //}
    //if (right.x > 1600)
    //{
    //    right_rgb = float4(0, 0, 0, 0);
    //}
    //else
    //{
    //    right_rgb = inputGlowData[right].rgba;
    //    calc_pxl++;
    //}
    ////float3 up_rgb = (up.y < 0) ? float3(0, 0, 0) : inputGlowData[up].rgb;
    ////float3 down_rgb = (down.y > 900) ? float3(0, 0, 0) : inputGlowData[down].rgb;
    ////float3 left_rgb = (left.x < 0) ? float3(0, 0, 0) : inputGlowData[left].rgb;
    ////float3 right_rgb = (right.x > 1600) ? float3(0, 0, 0) : inputGlowData[right].rgb;
    
    //float4 avg_rgb = float4(0, 0, 0, 0);
    
    ////blend colors
    ////calculate brightness?
    
    //[unroll]
    //for (int i = 0; i < 4; ++i)
    //{
    //    avg_rgb[i] = (3 * m_rgb[i] + up_rgb[i] + down_rgb[i] + left_rgb[i] + right_rgb[i]) / calc_pxl;
    //}
    //avg_rgb = float4(avg_rgb.rgb * avg_rgb.a, avg_rgb.a);
    
    ////avg_rgb.r += (up_rgb.r + down_rgb.r + left_rgb.r + right_rgb.r) / calc_pxl;
    ////avg_rgb.g += (up_rgb.g + down_rgb.g + left_rgb.g + right_rgb.g) / calc_pxl;
    ////avg_rgb.b += (up_rgb.b + down_rgb.b + left_rgb.b + right_rgb.b) / calc_pxl;
    ////avg_rgb.a += (up_rgb.a + down_rgb.a + left_rgb.a + right_rgb.a) / calc_pxl;
    
    ////avg_rgb.r = (instance == 0) ? avg_rgb.r : avg_rgb.r / instance;
    ////avg_rgb.g = (instance == 0) ? avg_rgb.g : avg_rgb.g / instance;
    ////avg_rgb.b = (instance == 0) ? avg_rgb.b : avg_rgb.b / instance;
    
    ////m_rgb += avg_rgb;
    
    ////m_rgb = clamp(m_rgb, float3(0, 0, 0), float3(1, 1, 1));
    
    //outputGlowData[index] = avg_rgb;    //WARNING: Might be issues with alpha being clamped! Maybe should depend on factor instead?
}