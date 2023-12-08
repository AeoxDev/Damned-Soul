#pragma once
#include "D3D11Helper/IDX_Types.h"
#include "ComponentHelper.h"

namespace Shatter
{
	extern CB_IDX cBuffer;
	extern GS_IDX gShader;

	void Initialize();
	void SetResources(ShatterComponent* comp);
	void UnsetResources();
}
