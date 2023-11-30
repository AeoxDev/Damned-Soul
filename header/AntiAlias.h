#pragma once
#include "D3D11Helper/D3D11Helper.h"
#include "GameRenderer.h"

namespace AntiAlias
{
	extern SRV_IDX backBufferCopy;
	//extern RTV_IDX backBufferResult;
	//extern DSV_IDX antiAliasDepth;
	extern CS_IDX antiAliasShader;

	extern UAV_IDX backBufferUAV;

	void Initialize();
	void PrepareAntiAliasPass();
	void FinishAntiAliasPass();
}