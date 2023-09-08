struct input
{
    float3 position;
    float padding1;
    float3 velocity;
    float padding2;
    float3 rgb;
    float padding3;

};


ConsumeStructuredBuffer<input> inputData : register(u0);
AppendStructuredBuffer<input> outputData : register(u1);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    
}