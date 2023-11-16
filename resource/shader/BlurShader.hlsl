Texture2D inputGlowData : register(t0);
RWTexture2D<float4> outputGlowData : register(u0);  // NOTE: float4?
// int power outside shader

[numthreads(32, 32, 1)]
void main( uint3 threadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{
    int2 index = int2(groupID.x * 32 + threadID.x, groupID.y * 32 + threadID.y);
    
    
    float3 m_rgb = inputGlowData[index.xy].rgb;
    
    int2 up = int2(index.x, index.y - 1);
    int2 down = int2(index.x, index.y + 1);
    int2 left = int2(index.x - 1, index.y);
    int2 right = int2(index.x + 1, index.y);
    
    float3 up_rgb = (up.y < 0) ? float3(0, 0, 0) : inputGlowData[up].rgb;
    float3 down_rgb = (down.y > 900) ? float3(0, 0, 0) : inputGlowData[down].rgb;
    float3 left_rgb = (left.x < 0) ? float3(0, 0, 0) : inputGlowData[left].rgb;
    float3 right_rgb = (right.x > 1600) ? float3(0, 0, 0) : inputGlowData[right].rgb;
    
    float3 avg_rgb = float3(0, 0, 0);
    
    avg_rgb.r += (up_rgb.r + down_rgb.r + left_rgb.r + right_rgb.r) / 4;
    avg_rgb.g += (up_rgb.g + down_rgb.g + left_rgb.g + right_rgb.g) / 4;
    avg_rgb.b += (up_rgb.b + down_rgb.b + left_rgb.b + right_rgb.b) / 4;
    
    m_rgb += avg_rgb;   // NOTE: Unsure of math.
    
    m_rgb = clamp(m_rgb, float3(0, 0, 0), float3(1, 1, 1));
    
    outputGlowData[index] = float4(m_rgb, 1.0f);
    //outputGlowData[index].rgb = m_rgb.rgb;
    //outputGlowData[index].a = 1.0f;
}