#pragma once
#include <cinttypes>
#define GI_TEXTURE_DIMENSIONS 2048
#define GI_TEXTURE_DIMENSIONS_FOR_PATHFINDING 512
#define GI_TEXTURE_DIMENSIONS_FOR_OBSTACLEAVOIDANCE 256

struct EntityID;

struct GeometryIndependentComponent;

struct GITexture
{
	int8_t texture[GI_TEXTURE_DIMENSIONS][GI_TEXTURE_DIMENSIONS];
};
extern GITexture* giTexture;


struct GIMapData
{
	int8_t texture[GI_TEXTURE_DIMENSIONS][GI_TEXTURE_DIMENSIONS];
};

//This takes in an EntityID for the stage, assuming there is an existing model for it.
//It also checks for each StaticHazardComponent with a TransformComponent and renders them as well.
void RenderGeometryIndependentCollision(EntityID& stageFloor, EntityID& walls, EntityID& hitbox);

//Functions for getting information:
int PixelValueOnPosition(GeometryIndependentComponent*& giComponent, TransformComponent* transform);
struct GridPosition
{
	int x;
	int z;

	float fx;
	float fz;
};

struct Coordinate2D
{
	float x;
	float z;
};

GridPosition PositionOnGrid(GeometryIndependentComponent*& giComponent, TransformComponent* transform, int dimensions = GI_TEXTURE_DIMENSIONS);
Coordinate2D GridOnPosition(GridPosition gridPos, GeometryIndependentComponent*& gi, int dimensions = GI_TEXTURE_DIMENSIONS);
GITexture* GetMapTexture(EntityID& entity);

void SetGISpawnPosition(float x, float z);