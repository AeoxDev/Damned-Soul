#pragma once

struct EntityID;

struct GeometryIndependentComponent;

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

GridPosition PositionOnGrid(GeometryIndependentComponent*& giComponent, TransformComponent*& transform);