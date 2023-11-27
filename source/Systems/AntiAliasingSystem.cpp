#include "Systems\Systems.h"
#include "GameRenderer.h"
#include "UIRenderer.h"
#include "AntiAlias.h"
#include "SDLHandler.h"

bool AntiAliasingSystem::Update()
{
	// set compute shader
	// copy backbuffer to SRV
	AntiAlias::backBufferCopy = CreateUnorderedAccessViewTexture(sdl.BASE_WIDTH, sdl.BASE_HEIGHT, renderStates[backBufferRenderSlot].renderTargetView);
	//CreateShaderResourceViewTexture()
	SetShaderResourceView(renderStates[ui.RenderSlot].shaderResourceView, BIND_COMPUTE, 0);
	// set backbufferSRV
	SetShaderResourceView(AntiAlias::backBufferCopy, BIND_COMPUTE, 0);
	// set backbuffer as RTV

	return false;
}