#pragma once
#include "D3D11Helper/IDX_Types.h"

namespace Glow
{
	extern SRV_IDX glow_srv;
	extern RTV_IDX glow_rtv;
	extern PS_IDX glow_shader;
	extern DSV_IDX glow_depth;
	extern CS_IDX blur_shader;
	extern UAV_IDX blur_uav;

	void Initialize();
	void PrepareGlowPass();
	void PrepareBlurPass();
	void FinishGlowPass();
	void FinishBlurPass();
	void ClearGlowRenderTarget();
}