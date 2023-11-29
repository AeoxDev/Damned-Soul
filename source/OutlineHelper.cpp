#include "OutlineHelper.h"
#include "GameRenderer.h"
#include "SDLHandler.h"

PS_IDX Outlines::outlinePixelShader = -1;
RTV_IDX Outlines::renderTarget = -1;
DSV_IDX Outlines::depthStencil = -1;
SRV_IDX Outlines::targetSRV = -1;
//UAV_IDX Outlines::targetUAV = -1;

//void Outlines::SwapTargets()
//{
//	CopyUAVToSRV(targetSRV, targetUAV);
//}
//
//void Outlines::SwapBack()
//{
//	CopySRVToUAV(targetUAV, targetSRV);
//}

void Outlines::InitializeOutlines()
{
	outlinePixelShader = LoadPixelShader("OutlinePixel.cso");
	renderTarget = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS(int(BIND_RENDER_TARGET) | int(BIND_SHADER_RESOURCE)), CPU_FLAGS::NONE, sdl.WIDTH, sdl.HEIGHT);
	depthStencil = CreateDepthStencil(sdl.WIDTH, sdl.HEIGHT);
	targetSRV = CreateShaderResourceViewTexture(renderTarget, RESOURCE_FLAGS::BIND_RENDER_TARGET);
	//targetUAV = CreateUnorderedAccessViewTexture(sdl.WIDTH, sdl.HEIGHT, renderStates[backBufferRenderSlot].renderTargetView);
}