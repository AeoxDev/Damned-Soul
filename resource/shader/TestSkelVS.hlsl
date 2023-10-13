cbuffer WorldMatrix : register (b0)
{
	matrix world;
}

cbuffer CameraBuffer : register(b1)
{
    float4 cameraPosition;
    matrix view;
    matrix projection;
}

cbuffer Skeleton : register(b2)
{
	matrix skeleMath[21];
}

struct VS_INPUTS
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float2 uv : UV;
	uint4 bIdx : BONE_INDEX;
	float4 bWeight : BONE_WIEGHT;
};

//struct VS_OUT
//{
//	float4 position : SV_POSITION;
//	float4 normal : NORMAL;
//	float2 uv : UV;
//	uint4 index : INDEX;
//	float4 weight : WEIGHT;
//    float4 camToWorldObject : CAM; // normalized //för att vsOut ska se likadan ut i båda vertexshaders 
//};

struct VS_OUT //VS_OUT needs to be exaxtly the same in all vertexshaders
{
    float4 position : SV_POSITION; //world, view, projection - multiplyed
    float4 normal : WNORMAL; //world - multiplyed
    float2 uv : UV;
    uint4 index : INDEX;
    float4 weight : WEIGHT;
    float4 camToWorldObject : CAM; // normalized 
    float4 world : WORLD;
};

VS_OUT main(VS_INPUTS pos)
{
	VS_OUT retval;

	matrix sTrans[4];
	sTrans[0] = skeleMath[pos.bIdx.x] * pos.bWeight.x;
	sTrans[1] = skeleMath[pos.bIdx.y] * pos.bWeight.y;
	sTrans[2] = skeleMath[pos.bIdx.z] * pos.bWeight.z;
	sTrans[3] = skeleMath[pos.bIdx.w] * pos.bWeight.w;
	matrix skeletonTransform = sTrans[0] + sTrans[1] + sTrans[2] + sTrans[3];

	retval.position = pos.position;
	
    retval.normal = mul(normalize(pos.normal), world); //
	retval.uv = pos.uv;
	
	retval.position = mul(retval.position, skeletonTransform);
	retval.position = mul(retval.position, world);
    retval.world = retval.position;
    retval.camToWorldObject = normalize(cameraPosition - retval.position); //använder världs position innan camera perspective
    retval.position = mul(retval.position, view);
    retval.position = mul(retval.position, projection);

	retval.index = pos.bIdx;
	retval.weight = pos.bWeight;

	return retval;
}

