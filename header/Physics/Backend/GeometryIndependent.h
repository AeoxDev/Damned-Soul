#pragma once
#include "D3D11Helper.h"
#include "EntityFramework.h"



RTV_IDX SetupGIRenderTargetView(Registry& registry, EntityID& stageEntity);
PS_IDX SetupGIPixelShader(Registry& registry, EntityID& stageEntity);
VS_IDX SetupGIVertexShader(Registry& registry, EntityID& stageEntity);
CB_IDX SetupGIConstantBuffer(Registry& registry, EntityID& stageEntity);
bool SetupGIAll(Registry& registry, EntityID& stageEntity);
//Creates a stage collision texture out of the modelcomponent in the given entity
void RenderGeometryIndependentCollisionToTexture(Registry& registry, EntityID& stageEntity);
bool AddGeometryIndependentComponent(Registry& registry, EntityID& stageEntity);
