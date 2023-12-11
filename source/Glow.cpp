#include "Glow.h"
#include "D3D11Helper/D3D11Helper.h"
#include "D3D11Helper\D3D11Graphics.h"
#include "SDLHandler.h"
#include "MemLib\MemLib.hpp"
#include "RenderDepthPass.h"
#include "UIRenderer.h"
#include "OutlineHelper.h"

SRV_IDX Glow::glow_srv;
UAV_IDX Glow::backbuffer_uav;
RTV_IDX Glow::bb_copy_rtv;
SRV_IDX Glow::bb_copy_srv;
RTV_IDX Glow::glow_rtv;
PS_IDX Glow::glow_shader;
DSV_IDX Glow::glow_depth;
CS_IDX Glow::blur_shader;
SRV_IDX Glow::blur_srv;
CB_IDX Glow::glow_buffer;
CB_IDX Glow::blur_buffer;
PoolPointer<Glow::GlowData> Glow::glow_bufData;
Glow::BlurData Glow::blur_bufData;
SRV_IDX Glow::blur_depth;

void Glow::Initialize()
{
	// Pixel
	glow_rtv = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS(RESOURCE_FLAGS::BIND_RENDER_TARGET | RESOURCE_FLAGS::BIND_SHADER_RESOURCE), CPU_FLAGS::NONE, sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	glow_srv = CreateShaderResourceViewTexture(glow_rtv, RESOURCE_FLAGS::BIND_RENDER_TARGET);
	glow_shader = LoadPixelShader("GlowShader.cso");
	glow_depth = CreateDepthStencil(sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	glow_buffer = CreateConstantBuffer(sizeof(GlowData));
	blur_buffer = CreateConstantBuffer(sizeof(BlurData));

	//bb_copy_rtv = CreateTexture(FORMAT_R32G32B32A32_FLOAT, USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS::BIND_SHADER_RESOURCE, CPU_FLAGS::WRITE, sdl.BASE_WIDTH, sdl.BASE_HEIGHT, true);
	bb_copy_rtv = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS(RESOURCE_FLAGS::BIND_RENDER_TARGET | RESOURCE_FLAGS::BIND_SHADER_RESOURCE), CPU_FLAGS::NONE, sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
	bb_copy_srv = CreateShaderResourceViewTexture(bb_copy_rtv, RESOURCE_FLAGS::BIND_RENDER_TARGET);

	// Compute
	blur_shader = LoadComputeShader("BlurShader.cso");
	backbuffer_uav = CreateUnorderedAccessViewTexture(sdl.BASE_WIDTH, sdl.BASE_HEIGHT, renderStates[backBufferRenderSlot].renderTargetView);
	//blur_depth = CreateShaderResourceViewTexture(blur_depth, RESOURCE_FLAGS::BIND_DEPTH_STENCIL); // TODO: Make sure I can create an srv for glow_depth
}

void Glow::PrepareGlowPass()
{
	CopyUAVToSRV(bb_copy_srv, backbuffer_uav);
	SetRenderTargetViewAndDepthStencil(glow_rtv, glow_depth);
	SetPixelShader(glow_shader);
}

void Glow::FinishGlowPass()
{
	UnsetPixelShader();
	UnsetRenderTargetViewAndDepthStencil();
	UnsetConstantBuffer(BIND_PIXEL, 2);
}

void Glow::SetBlurViews()
{
	SetUnorderedAcessView(backbuffer_uav, 0);
	SetShaderResourceView(renderStates[ui.RenderSlot].shaderResourceView, BIND_COMPUTE, 0);
	SetShaderResourceView(glow_srv, BIND_COMPUTE, 1);
	SetShaderResourceView(Outlines::targetSRV, BIND_COMPUTE, 2);
	SetShaderResourceView(bb_copy_srv, BIND_COMPUTE, 3);
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

void Glow::UpdateBlurBuffer()
{
	UnsetConstantBuffer(BIND_COMPUTE, 0);
	
	blur_bufData.windowWidth = sdl.BASE_WIDTH;
	blur_bufData.windowHeight = sdl.BASE_HEIGHT;
	UpdateConstantBuffer(blur_buffer, &blur_bufData);
	
	SetConstantBuffer(blur_buffer, BIND_COMPUTE, 0);
}

void Glow::PrepareBlurPass()
{
	SetComputeShader(blur_shader);
	SetBlurViews();
}

void Glow::FinishBlurPass()
{
	UnsetUnorderedAcessView(0);
	SetDepthPassTextureCompute(false);
	UnsetShaderResourceView(BIND_COMPUTE, 0);
	UnsetShaderResourceView(BIND_COMPUTE, 1);
	UnsetShaderResourceView(BIND_COMPUTE, 2);
	UnsetShaderResourceView(BIND_COMPUTE, 3);

	UnsetConstantBuffer(BIND_COMPUTE, 0);
	UnsetComputeShader();
}

void Glow::ClearGlowRenderTarget()
{
	ClearRenderTargetView(glow_rtv, 0, 0, 0, 0);
	ClearDepthStencilView(glow_depth);
}
