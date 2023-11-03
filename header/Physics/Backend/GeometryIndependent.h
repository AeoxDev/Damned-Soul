#pragma once
#include "D3D11Helper\D3D11Helper.h"
#include "EntityFramework.h"
#include "EntityID.h"
#define TEXTURE_DIMENSIONS 512
struct GITexture
{
	int8_t texture[TEXTURE_DIMENSIONS][TEXTURE_DIMENSIONS];
};
extern GITexture* giTexture;

RTV_IDX SetupGIRenderTargetView(EntityID& stageEntity);
PS_IDX SetupGIPixelShader(EntityID& stageEntity);
VS_IDX SetupGIVertexShader(EntityID& stageEntity);
CB_IDX SetupGIConstantBuffer(EntityID& stageEntity);

void CreateGITexture();

bool SetupGIAll(EntityID& stageEntity);
//Creates a stage collision texture out of the modelcomponent in the given entity
//void RenderGeometryIndependentCollisionToTexture(EntityID& stageEntity);
bool AddGeometryIndependentComponent(EntityID& stageEntity);
void ReleaseGI(EntityID& entity);

