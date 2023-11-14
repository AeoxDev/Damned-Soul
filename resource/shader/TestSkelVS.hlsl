#include "RenderPipelineHeader.hlsli"

VS_OUT main(VS_INPUTS_SKELETON pos)
{
	VS_OUT retval;

    matrix skeletonTransform = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    matrix skeletonTransformNormal = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        skeletonTransform += skeleMath[pos.bIdx[i]] * pos.bWeight[i];
        skeletonTransformNormal += skeleMathNormal[pos.bIdx[i]] * pos.bWeight[i];
    }
	//matrix sTrans[4];
 //   sTrans[0] = skeleMath[pos.bIdx[0]] * pos.bWeight[0];
 //   sTrans[1] = skeleMath[pos.bIdx[1]] * pos.bWeight[1];
 //   sTrans[2] = skeleMath[pos.bIdx[2]] * pos.bWeight[2];
 //   sTrans[3] = skeleMath[pos.bIdx[3]] * pos.bWeight[3];
	//matrix skeletonTransform = sTrans[0] + sTrans[1] + sTrans[2] + sTrans[3];

	retval.position = pos.base.position;
	
    retval.normal = mul(normalize(pos.base.normal), skeletonTransformNormal);
    retval.normal = mul(pos.base.normal, worldNormal);
	retval.uv = pos.base.uv;
	
	retval.position = mul(retval.position, skeletonTransform);
	retval.position = mul(retval.position, world);
    retval.world = retval.position;
    retval.camToWorldObject = normalize(cameraPosition - retval.position); //använder världs position innan camera perspective
    retval.position = mul(retval.position, view);
    retval.position = mul(retval.position, projection);
    retval.depth.x = retval.position.z;
    retval.depth.y = retval.position.w;
	return retval;
}

