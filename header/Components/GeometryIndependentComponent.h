#pragma once
#include "EntityFramework.h"

struct GeometryIndependentComponent;

EntityID CreateAndRenderGeometryIndependentCollision(EntityID& modelEntity);
void RenderGeometryIndependentCollision(EntityID& m);

//Functions for getting information:
int PixelValueOnPosition(GeometryIndependentComponent*& giComponent, float x, float z);