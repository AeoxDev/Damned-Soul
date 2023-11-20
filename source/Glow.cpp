#include "Glow.h"
#include "D3D11Helper/D3D11Helper.h"
#include "SDLHandler.h"
#include "MemLib\MemLib.hpp"

SRV_IDX Glow::glow_srv;
UAV_IDX Glow::backbuffer_uav;
RTV_IDX Glow::glow_rtv;
PS_IDX Glow::glow_shader;
DSV_IDX Glow::glow_depth;
CS_IDX Glow::blur_shader;
UAV_IDX Glow::blur_uav1;
UAV_IDX Glow::blur_uav2;
CB_IDX Glow::blur_buffer;
PoolPointer<Glow::BlurData> Glow::blur_bufData;

void Glow::Initialize()
{
	// Pixel
	glow_rtv = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS(RESOURCE_FLAGS::BIND_RENDER_TARGET | RESOURCE_FLAGS::BIND_SHADER_RESOURCE), CPU_FLAGS::NONE, sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	glow_srv = CreateShaderResourceViewTexture(glow_rtv, RESOURCE_FLAGS::BIND_RENDER_TARGET);
	glow_shader = LoadPixelShader("GlowShader.cso");
	glow_depth = CreateDepthStencil(sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	

	// Compute
	blur_shader = LoadComputeShader("BlurShader.cso");
	blur_uav1 = CreateUnorderedAccessViewTexture(sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	blur_uav2 = CreateUnorderedAccessViewTexture(sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	blur_buffer = CreateConstantBuffer(sizeof(BlurData));

	backbuffer_uav = CreateUnorderedAccessViewTexture(sdl.BASE_WIDTH, sdl.BASE_HEIGHT, renderStates[backBufferRenderSlot].renderTargetView);
}

void Glow::PrepareGlowPass()
{
	SetRenderTargetViewAndDepthStencil(glow_rtv, glow_depth);
	SetPixelShader(glow_shader);
}

void Glow::SetViews()
{
	//UnsetUnorderedAcessView(0);
	//UnsetUnorderedAcessView(1);
	//UnsetUnorderedAcessView(2);
	SetUnorderedAcessView(blur_uav1, 0);
	SetUnorderedAcessView(blur_uav2, 1);
	SetUnorderedAcessView(backbuffer_uav, 2);
}

void Glow::FinishGlowPass()
{
	UnsetPixelShader();
	UnsetRenderTargetViewAndDepthStencil();
	// NOTE: ?
	
	CopySRVToUAV(blur_uav2, glow_srv);
	CopySRVToUAV(blur_uav1, glow_srv);
}

void Glow::SwitchUAV()
{
	UAV_IDX tmp = blur_uav2;
	blur_uav2 = blur_uav1;
	blur_uav1 = tmp;
}

void Glow::UpdateBlurBuffer(int instance)
{
	if (instance != 0)
	{
		UnsetConstantBuffer(BIND_COMPUTE, 0);
	}
	blur_bufData = MemLib::palloc(sizeof(BlurData));
	blur_bufData->instance = instance;
	UpdateConstantBuffer(blur_buffer, blur_bufData);
	MemLib::pfree(blur_bufData);
	SetConstantBuffer(blur_buffer, BIND_COMPUTE, 0);
}

void Glow::PrepareBlurPass()
{
	SetComputeShader(blur_shader);
	SetViews();
}

void Glow::FinishBlurPass()
{
	UnsetUnorderedAcessView(0);
	UnsetUnorderedAcessView(1);
	UnsetUnorderedAcessView(2);
	UnsetComputeShader();
	UnsetConstantBuffer(BIND_COMPUTE, 0);

	CopyUAVToSRV(glow_srv, blur_uav1);
}

void Glow::ClearGlowRenderTarget()
{
	ClearRenderTargetView(glow_rtv, 0, 0, 0, 0); // NOTE: Alpha?
	ClearDepthStencilView(glow_depth);
}
