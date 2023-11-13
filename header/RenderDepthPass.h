#pragma once
#include "D3D11Helper\IDX_Types.h"

//This files contains the nescessary functions for render the depth pass for forward +
//As well as the depth pass for shadow mapping

PS_IDX CreateDepthPassPixelShader(char* shader);
PS_IDX GetDepthPassPixelShader();

void CreateDepthPass();
void SetDepthPassTexture(bool forRendering);
void UnsetDepthPassTexture(bool forRendering);
void ReleaseDepthPass();

//Create the viewport, depthstencil, texture, render target and shader resource for the shadow map.
void CreateShadowMap(const unsigned& width, const unsigned& height);
void SetShadowmap(bool forRendering);
void UnsetShadowmap(bool forRendering);
PS_IDX GetShadowMapPixelShader();