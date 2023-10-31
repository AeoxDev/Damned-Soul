#pragma once
#include <cinttypes>
#define GI_TEXTURE_DIMENSIONS 128
struct EntityID;

struct GeometryIndependentComponent;

struct GIMapData
{
	int8_t texture[GI_TEXTURE_DIMENSIONS][GI_TEXTURE_DIMENSIONS];
};

EntityID CreateAndRenderGeometryIndependentCollision(EntityID& modelEntity);
void RenderGeometryIndependentCollision(EntityID& m);

//Functions for getting information:
int PixelValueOnPosition(GeometryIndependentComponent*& giComponent, float x, float z);

GIMapData* GetMapTexture(EntityID& entity);