#pragma once
#include "D3D11Helper\D3D11Helper.h"

extern PS_IDX outlinePixelShader;
extern RTV_IDX outlineRTV;
extern DSV_IDX outlineDSV;

void ValidateOutlineResources();