#pragma once
#include "D3D11Helper.h"
#include "MemLib\ML_Vector.hpp"
#include "GameRenderer.h"

struct RenderableComponent
{
private:
	VS_IDX* vsIdx = nullptr;
	PS_IDX* psIdx = nullptr;
	GS_IDX* gsIdx = nullptr;
	CS_IDX* csIdx = nullptr;
	VB_IDX* vbIdx = nullptr;
	IB_IDX* ibIdx = nullptr;
	VP_IDX*	vpIdx = nullptr;
	UAV_IDX* uavIdx = nullptr;
	RS_IDX*	rsIdx = nullptr;
	ML_Vector<CB_IDX*> cbIdx;
	ML_Vector<RTV_IDX*> rtvIdx;
	ML_Vector<DSV_IDX*> dsvIdx;
	ML_Vector<SRV_IDX*> srvIdx;
public:
	void ToRenderableComponent(RenderSetupComponent& setupComponent)
	{
		vsIdx = &setupComponent.vertexShader;
		psIdx = &setupComponent.pixelShader;
		gsIdx = &setupComponent.geometryShader;
		csIdx = &setupComponent.computeShader;
		vbIdx = &setupComponent.vertexBuffer;
		ibIdx = &setupComponent.indexBuffer;
		vpIdx = &setupComponent.viewPort;
		uavIdx = &setupComponent.unorderedAccessView;
		rsIdx = &setupComponent.rasterizerState;
		cbIdx.push_back(&setupComponent.constantBuffer);
		rtvIdx.push_back(&setupComponent.renderTargetView);
		dsvIdx.push_back(&setupComponent.depthStencilView);
		srvIdx.push_back(&setupComponent.shaderResourceView);
	}
};

