#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "GameRenderer.h"
#include "Particles.h"
#include "D3D11Helper\D3D11Helper.h"
#include "RenderDepthPass.h"


bool ParticleSystem::Update()
{
	//PrepareBackBuffer();

	SetRenderTargetViewAndDepthStencil(renderStates[backBufferRenderSlot].renderTargetView, renderStates[backBufferRenderSlot].depthStencilView);

	SetVertexShader(renderStates[Particles::RenderSlot].vertexShaders[1]);
	SetPixelShader(renderStates[Particles::RenderSlot].pixelShaders[1]);
	//SetRenderTargetViewAndDepthStencil(renderStates[backBufferRenderSlot].renderTargetView, renderStates[backBufferRenderSlot].depthStencilView);

	SetVertexBuffer(renderStates[backBufferRenderSlot].vertexBuffer);
	SetIndexBuffer(renderStates[backBufferRenderSlot].indexBuffer);
	SetShaderResourceView(renderStates[Particles::RenderSlot].shaderResourceView, BIND_PIXEL, 2);
	//SetDepthPassTexture(false);
	SetShaderResourceView(Particles::depthSRV, BIND_PIXEL, 4);

	RenderIndexed(3);

	//UNBIND SHIT 
	UnsetShaderResourceView(BIND_PIXEL, 2);
	//UnsetDepthPassTexture(false);
	UnsetShaderResourceView(BIND_PIXEL, 4);
	return true;
}