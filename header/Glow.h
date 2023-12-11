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

	struct BlurData
	{
		int windowWidth;
		int windowHeight;
	};

	// TODO: Edit resources. UAV is unnecessary if only read access is needed.

	extern SRV_IDX glow_srv;
	extern UAV_IDX backbuffer_uav;
	extern RTV_IDX bb_copy_rtv;
	extern SRV_IDX bb_copy_srv;
	extern RTV_IDX glow_rtv;
	extern PS_IDX glow_shader;
	extern DSV_IDX glow_depth;
	extern CS_IDX blur_shader;
	extern SRV_IDX blur_srv;
	extern CB_IDX glow_buffer;
	extern CB_IDX blur_buffer;
	extern PoolPointer<GlowData> glow_bufData;
	extern BlurData blur_bufData;
	extern SRV_IDX blur_depth;

	// Create and load everything necessary for glow to work.
	void Initialize();
	// Set recources for glow pass.
	void PrepareGlowPass();
	// Unset resources used in glow pass.
	void FinishGlowPass();
	// Set views necessary for blur pass.
	void SetBlurViews();
	// Update with values from GlowComponent.
	void UpdateGlowBuffer(float r, float g, float b);
	// Update with window values for correct resolution.
	void UpdateBlurBuffer();
	// Set resources for blur pass.
	void PrepareBlurPass();
	// Unset resources used in blur pass.
	void FinishBlurPass();
	// Clear glow texture.
	void ClearGlowRenderTarget();
}