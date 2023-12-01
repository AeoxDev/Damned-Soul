#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "GameRenderer.h"
#include "Particles.h"
#include "D3D11Helper\D3D11Helper.h"
#include "RenderDepthPass.h"
#include "Glow.h"


bool ParticleSystem::Update()
{
	////PrepareBackBuffer();

	ClearDepthStencilView(Particles::proxyDepth);

	SetRenderTargetViewAndDepthStencil(renderStates[backBufferRenderSlot].renderTargetView, Particles::proxyDepth);

	SetVertexShader(renderStates[Particles::RenderSlot].vertexShaders[1]);
	SetPixelShader(renderStates[Particles::RenderSlot].pixelShaders[1]);

	//////Set the depth texture


	SetVertexBuffer(renderStates[backBufferRenderSlot].vertexBuffer);
	SetIndexBuffer(renderStates[backBufferRenderSlot].indexBuffer);
	SetShaderResourceView(renderStates[Particles::RenderSlot].shaderResourceView, BIND_PIXEL, 2);
	//SetShaderResourceView(renderStates[Particles::RenderSlot].shaderResourceView, BIND_PIXEL, 3);
	SetShaderResourceView(Particles::backBufferDepthSRV, BIND_PIXEL, 3);
	SetShaderResourceView(Particles::particleDepthSRV, BIND_PIXEL, 4);

	RenderIndexed(3);

	//////UNBIND SHIT 
	UnsetShaderResourceView(BIND_PIXEL, 2);
	UnsetShaderResourceView(BIND_PIXEL, 3);
	//UnsetDepthPassTexture(false);
	UnsetShaderResourceView(BIND_PIXEL, 4);
	return true;
}