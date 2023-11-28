#include "OutlineHelper.h"
#include "SDLHandler.h"

PS_IDX outlinePixelShader = -1;
RTV_IDX outlineRTV;
DSV_IDX outlineDSV;

void ValidateOutlineResources()
{
	// Load the shader if it isn't loaded
	if (-1 == outlinePixelShader || -1 == outlineRTV || -1 == outlineDSV)
	{
		outlinePixelShader = LoadPixelShader("OutlinePixel.cso");
		outlineRTV = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS(int(BIND_RENDER_TARGET) | int(BIND_SHADER_RESOURCE)), CPU_FLAGS::NONE, sdl.WIDTH, sdl.HEIGHT);
		outlineDSV = CreateDepthStencil(sdl.WIDTH, sdl.HEIGHT);
	}
}