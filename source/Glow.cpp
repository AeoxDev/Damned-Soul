#include "Glow.h"
#include "D3D11Helper/D3D11Helper.h"
#include "SDLHandler.h"
#include "MemLib\MemLib.hpp"
#include "RenderDepthPass.h"

SRV_IDX Glow::glow_srv;
UAV_IDX Glow::backbuffer_uav;
RTV_IDX Glow::glow_rtv;
PS_IDX Glow::glow_shader;
DSV_IDX Glow::glow_depth;
CS_IDX Glow::blur_shader;
UAV_IDX Glow::blur_uav1;
UAV_IDX Glow::blur_uav2;
CB_IDX Glow::glow_buffer;
PoolPointer<Glow::GlowData> Glow::glow_bufData;

void Glow::Initialize()
{
	// Pixel
	glow_rtv = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS(RESOURCE_FLAGS::BIND_RENDER_TARGET | RESOURCE_FLAGS::BIND_SHADER_RESOURCE), CPU_FLAGS::NONE, sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	glow_srv = CreateShaderResourceViewTexture(glow_rtv, RESOURCE_FLAGS::BIND_RENDER_TARGET);
	glow_shader = LoadPixelShader("GlowShader.cso");
	glow_depth = CreateDepthStencil(sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	glow_buffer = CreateConstantBuffer(sizeof(GlowData));
	

	// Compute
	blur_shader = LoadComputeShader("BlurShader.cso");
	blur_uav1 = CreateUnorderedAccessViewTexture(sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	blur_uav2 = CreateUnorderedAccessViewTexture(sdl.BASE_WIDTH, sdl.BASE_HEIGHT);

	backbuffer_uav = CreateUnorderedAccessViewTexture(sdl.BASE_WIDTH, sdl.BASE_HEIGHT, renderStates[backBufferRenderSlot].renderTargetView);
}

void Glow::PrepareGlowPass()
{
	SetRenderTargetViewAndDepthStencil(glow_rtv, glow_depth);
	SetPixelShader(glow_shader);
}

void Glow::FinishGlowPass()
{
	UnsetPixelShader();
	UnsetRenderTargetViewAndDepthStencil();
	CopySRVToUAV(blur_uav2, glow_srv);
	CopySRVToUAV(blur_uav1, glow_srv);
	UnsetConstantBuffer(BIND_PIXEL, 2);
}

void Glow::SetBlurViews()
{
	SetUnorderedAcessView(blur_uav1, 0);
	SetUnorderedAcessView(blur_uav2, 1);
	SetUnorderedAcessView(backbuffer_uav, 2);
	SetDepthPassTextureCompute(true);
}

void Glow::UpdateGlowBuffer(float r, float g, float b)
{
	UnsetConstantBuffer(BIND_PIXEL, 2);
	glow_bufData = MemLib::palloc(sizeof(GlowData));
	glow_bufData->col_r = r;
	glow_bufData->col_g = g;
	glow_bufData->col_b = b;
	UpdateConstantBuffer(glow_buffer, glow_bufData);
	MemLib::pfree(glow_bufData);
	SetConstantBuffer(glow_buffer, BIND_PIXEL, 2);
}

void Glow::PrepareBlurPass()
{
	SetComputeShader(blur_shader);
	SetBlurViews();
}

void Glow::FinishBlurPass()
{
	UnsetUnorderedAcessView(0);
	UnsetUnorderedAcessView(1);
	UnsetUnorderedAcessView(2);
	SetDepthPassTextureCompute(false);
	UnsetComputeShader();

	CopyUAVToSRV(glow_srv, blur_uav1);
}

void Glow::ClearGlowRenderTarget()
{
	ClearRenderTargetView(glow_rtv, 0, 0, 0, 0); // NOTE: Alpha?
	ClearDepthStencilView(glow_depth);
}
