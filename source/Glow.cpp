#include "Glow.h"
#include "D3D11Helper/D3D11Helper.h"
#include "SDLHandler.h"

SRV_IDX Glow::glow_srv;
RTV_IDX Glow::glow_rtv;
PS_IDX Glow::glow_shader;
DSV_IDX Glow::glow_depth;
CS_IDX Glow::blur_shader;
UAV_IDX Glow::blur_uav;

void Glow::Initialize()
{
	// Pixel
	glow_rtv = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS(RESOURCE_FLAGS::BIND_RENDER_TARGET | RESOURCE_FLAGS::BIND_SHADER_RESOURCE), CPU_FLAGS::NONE, sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	glow_srv = CreateShaderResourceViewTexture(glow_rtv, RESOURCE_FLAGS::BIND_RENDER_TARGET);
	glow_shader = LoadPixelShader("GlowShader.cso");
	glow_depth = CreateDepthStencil(sdl.BASE_WIDTH, sdl.BASE_HEIGHT);

	// Compute

	blur_shader = LoadComputeShader("BlurShader.cso");
}

void Glow::PrepareGlowPass()
{
	SetRenderTargetViewAndDepthStencil(glow_rtv, glow_depth);
	SetPixelShader(glow_shader);
}

void Glow::PrepareBlurPass()
{
	SetComputeShader(blur_shader);
	SetShaderResourceView(glow_srv, BIND_COMPUTE, 0);		// WARNING: Might be reserved
	SetUnorderedAcessView(blur_uav, 0);
}

void Glow::FinishGlowPass()
{
	UnsetPixelShader();
	UnsetRenderTargetViewAndDepthStencil();
	// NOTE: ?
}

void Glow::FinishBlurPass()
{
	UnsetUnorderedAcessView(0);
	UnsetShaderResourceView(BIND_COMPUTE, 0);
	UnsetComputeShader();

}

void Glow::ClearGlowRenderTarget()
{
	ClearRenderTargetView(glow_rtv, 0, 0, 0, 1); // NOTE: Alpha?
	ClearDepthStencilView(glow_depth);
}
