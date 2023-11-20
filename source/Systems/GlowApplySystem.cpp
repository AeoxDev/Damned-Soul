#include "Systems\Systems.h"
#include "D3D11Helper\D3D11Helper.h"
#include "Glow.h"
#include "Particles.h"


bool GlowApplySystem::Update()
{
	SetRenderTargetViewAndDepthStencil(renderStates[backBufferRenderSlot].renderTargetView, renderStates[backBufferRenderSlot].depthStencilView);
	PrepareBackBuffer();

	SetVertexShader(renderStates[Particles::RenderSlot].vertexShaders[1]);
	SetPixelShader(renderStates[Particles::RenderSlot].pixelShaders[1]);

	SetVertexBuffer(renderStates[backBufferRenderSlot].vertexBuffer);
	SetIndexBuffer(renderStates[backBufferRenderSlot].indexBuffer);
	SetShaderResourceView(Glow::glow_srv, BIND_PIXEL, 3);

	RenderIndexed(3);

	UnsetRenderTargetViewAndDepthStencil();
	UnsetVertexShader();
	UnsetPixelShader();
	UnsetVertexBuffer();
	UnsetIndexBuffer();
	UnsetShaderResourceView(BIND_PIXEL, 3);

	return true;
}