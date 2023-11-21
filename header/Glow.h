#pragma once
#include "D3D11Helper/IDX_Types.h"
#include "MemLib\PoolPointer.hpp"
#include "GameRenderer.h"


namespace Glow
{
	struct GlowData
	{
		float col_r;
		float col_g;
		float col_b;
	};

	extern SRV_IDX glow_srv;
	extern UAV_IDX backbuffer_uav;
	extern RTV_IDX glow_rtv;
	extern PS_IDX glow_shader;
	extern DSV_IDX glow_depth;
	extern CS_IDX blur_shader;
	extern UAV_IDX blur_uav1;
	extern UAV_IDX blur_uav2;
	extern CB_IDX glow_buffer;
	extern PoolPointer<GlowData> glow_bufData;

	void Initialize();
	void PrepareGlowPass();
	void SetViews();
	void FinishGlowPass();
	void SwitchUAV();
	void UpdateGlowBuffer(float r, float g, float b);
	void PrepareBlurPass();
	void FinishBlurPass();
	void ClearGlowRenderTarget();
}