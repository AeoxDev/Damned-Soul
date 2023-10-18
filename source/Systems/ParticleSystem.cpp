#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "GameRenderer.h"
#include "Particles.h"
#include "D3D11Helper.h"



bool ParticleSystem::Update()
{
	PrepareBackBuffer();

	SetVertexShader(renderStates[Particles::RenderSlot].vertexShaders[1]);
	SetPixelShader(renderStates[Particles::RenderSlot].pixelShaders[1]);

	SetVertexBuffer(renderStates[backBufferRenderSlot].vertexBuffer);
	SetIndexBuffer(renderStates[backBufferRenderSlot].indexBuffer);
	SetShaderResourceView(renderStates[Particles::RenderSlot].shaderResourceView, BIND_PIXEL, 3);

	RenderIndexed(3);

	//UNBIND SHIT 
	UnsetShaderResourceView(BIND_PIXEL, 3);
	return true;
}