#include "D3D11Graphics.h"

#define FIND_NEXT_SUITABLE(main_map, var)			\
uint32_t infinityLoopCheck = 0;						\
const uint32_t stopValue = 2 << (sizeof(var) / 2);	\
while (_nextIdx < 0 || main_map.contains(_nextIdx))	\
{													\
	++_nextIdx;										\
	_nextIdx = _nextIdx * (1 - (_nextIdx < 0));		\
	if (stopValue == ++infinityLoopCheck)			\
		std::terminate();							\
}													\

GS_IDX GeometryShaderHolder::NextIdx()
{
	//uint32_t infinityLoopCheck = 0;
	//// Find suitable index
	//while (_nextIdx < 0 || gs_map.contains(_nextIdx))
	//{
	//	// Increment
	//	++_nextIdx;
	//	// Branchlessly set to 0 if negative
	//	_nextIdx = _nextIdx * (1 - (_nextIdx < 0));
	//	if (UINT32_MAX == ++infinityLoopCheck)
	//		std::terminate();
	//}

	FIND_NEXT_SUITABLE(gs_map, GS_IDX);

	return _nextIdx;
}

PS_IDX PixelShaderHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(ps_map, PS_IDX);

	return _nextIdx;
}

VS_IDX VertexShaderHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(vs_map, VS_IDX);

	return _nextIdx;
}

CS_IDX ComputeShaderHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(cs_map, CS_IDX);

	return _nextIdx;
}

TX_IDX TextureHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(tx_map, TX_IDX);

	return _nextIdx;
}

SMP_IDX SamplerStateHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(smp_map, SMP_IDX);

	return _nextIdx;
}

uint16_t BufferHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(buff_map, uint16_t);

	return _nextIdx;
}

VP_IDX ViewPortHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(vp_map, VP_IDX);

	return _nextIdx;
}

RTV_IDX RTVHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(rtv_map, RTV_IDX);

	return _nextIdx;
}

DSV_IDX DSVHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(dsv_map, DSV_IDX);

	return _nextIdx;
}

SRV_IDX SRVHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(srv_map, SRV_IDX);

	return _nextIdx;
}

UAV_IDX UAVHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(uav_map, UAV_IDX);

	return _nextIdx;
}

RS_IDX RasterizerHolder::NextIdx()
{
	FIND_NEXT_SUITABLE(rs_map, RS_IDX);

	return _nextIdx;
}