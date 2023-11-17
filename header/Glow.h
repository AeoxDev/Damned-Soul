#pragma once
#include "D3D11Helper/IDX_Types.h"
#include "MemLib\PoolPointer.hpp"

namespace Glow
{
	struct BlurData
	{
		int instance = 0;
	};

	extern SRV_IDX glow_srv;
	extern RTV_IDX glow_rtv;
	extern PS_IDX glow_shader;
	extern DSV_IDX glow_depth;
	extern CS_IDX blur_shader;
	extern UAV_IDX blur_uav1;
	extern UAV_IDX blur_uav2;
	extern CB_IDX blur_buffer;
	extern PoolPointer<BlurData> blur_bufData;

	void Initialize();
	void PrepareGlowPass();
	void SetViews();
	void FinishGlowPass();
	void SwitchUAV();
	void UpdateBlurBuffer(int instance);
	void PrepareBlurPass();
	void FinishBlurPass();
	void ClearGlowRenderTarget();
}