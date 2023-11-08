#include "RenderPipelineHeader.hlsli"

VS_OUT main(VS_INPUTS_SKELETON pos)
{
	VS_OUT retval;

	matrix sTrans[4];
	sTrans[0] = skeleMath[pos.bIdx.x] * pos.bWeight.x;
	sTrans[1] = skeleMath[pos.bIdx.y] * pos.bWeight.y;
	sTrans[2] = skeleMath[pos.bIdx.z] * pos.bWeight.z;
	sTrans[3] = skeleMath[pos.bIdx.w] * pos.bWeight.w;
	matrix skeletonTransform = sTrans[0] + sTrans[1] + sTrans[2] + sTrans[3];

	retval.position = pos.base.position;
	
    retval.normal = normalize(mul(pos.base.normal, worldNormal)); //
	retval.uv = pos.base.uv;
	
	retval.position = mul(retval.position, skeletonTransform);
	retval.position = mul(retval.position, world);
    retval.world = retval.position;
    retval.camToWorldObject = normalize(cameraPosition - retval.position); //använder världs position innan camera perspective
    retval.position = mul(retval.position, view);
    retval.position = mul(retval.position, projection);

	return retval;
}

