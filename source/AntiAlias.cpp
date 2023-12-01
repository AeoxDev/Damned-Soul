#include "AntiAlias.h"
#include "D3D11Helper/D3D11Helper.h"
#include "SDLHandler.h"
#include "UIRenderer.h"

SRV_IDX AntiAlias::backBufferCopy;
CS_IDX AntiAlias::antiAliasShader;
UAV_IDX AntiAlias::backBufferUAV;

void AntiAlias::Initialize()
{
	antiAliasShader = LoadComputeShader("AntiAliasingShader.cso");
	backBufferUAV = CreateUnorderedAccessViewTexture(sdl.BASE_WIDTH, sdl.BASE_HEIGHT, renderStates[backBufferRenderSlot].renderTargetView);
	//backBufferCopy = CreateShaderResourceViewTexture(RESOURCES::RESOURCE_TEXTURE2D, RESOURCE_FLAGS::BIND_SHADER_RESOURCE, CPU_FLAGS::READ, sdl.BASE_WIDTH, sdl.BASE_HEIGHT);
}

void AntiAlias::PrepareAntiAliasPass()
{
	//CopyUAVToSRV(backBufferCopy, backBufferUAV);		// HELP: Causes visual issues!!!

	//SetComputeShader(antiAliasShader);
	//SetShaderResourceView(backBufferCopy, BIND_COMPUTE, 0);
	//SetShaderResourceView(renderStates[ui.RenderSlot].shaderResourceView, BIND_COMPUTE, 1);
	//SetUnorderedAcessView(backBufferUAV, 0);
	
}

void AntiAlias::FinishAntiAliasPass()
{
	//UnsetComputeShader();
	//UnsetShaderResourceView(BIND_COMPUTE, 0);
	//UnsetShaderResourceView(BIND_COMPUTE, 1);
	//UnsetUnorderedAcessView(0);
}
