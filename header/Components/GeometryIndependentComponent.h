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
//This takes in an EntityID for the stage, assuming there is an existing model for it.
//It also checks for each StaticHazardComponent with a TransformComponent and renders them as well.
void RenderGeometryIndependentCollision(EntityID& stageEntity);

//Functions for getting information:
int PixelValueOnPosition(GeometryIndependentComponent*& giComponent, TransformComponent*& transform);
struct GridPosition
{
	int x;
	int z;
};

struct Coordinate2D
{
	float x;
	float z;
};

GridPosition PositionOnGrid(GeometryIndependentComponent*& giComponent, TransformComponent*& transform);
Coordinate2D GridOnPosition(GridPosition gridPos, GeometryIndependentComponent*& gi);
GIMapData* GetMapTexture(EntityID& entity);