#pragma once
#include "D3D11Helper\D3D11Helper.h"

namespace Outlines
{
	extern PS_IDX outlinePixelShader;
	extern RTV_IDX renderTarget;
	extern DSV_IDX depthStencil;
	extern SRV_IDX targetSRV;
	extern UAV_IDX targetUAV;

	//void SwapTargets();
	//void SwapBack();

	void InitializeOutlines();
}