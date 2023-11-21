#pragma once
#include "EntityFramework.h"
//Real

//CIRCLE COLLISION:

/// <summary>
/// Create a circle hitbox on the given entity with a radius and offset.
/// The offset does not compensate for the rotation of the entity.
/// </summary>
/// <param name="radius">The size of the circle</param>
/// <param name="offsetX"></param>
/// <param name="offsetZ"></param>
/// <param name="entity"></param>
/// <returns>The id of the circle, use this to edit and destroy this hitbox for the given component
/// Returns -1 if component does not exist
/// -2 if Maximum amount of hitboxes achieved</returns>
//int CreateHitbox(EntityID& entity, float radius, float offsetX, float offsetZ);
//void RemoveHitbox( EntityID& entity, int hitboxID);
////Manual Flag setting functions.
//
//void SetHitboxActive( EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxIsStage( EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxIsWall( EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxIsPlayer(  EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxIsEnemy(  EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxIsStaticHazard(  EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxIsDynamicHazard(  EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxIsMoveable( EntityID& entity, int hitboxID, bool setFlag = true);
//
//void SetHitboxHitStage( EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxHitWall( EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxHitPlayer(EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxHitEnemy(EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxHitStaticHazard(EntityID& entity, int hitboxID, bool setFlag = true);
//void SetHitboxHitDynamicHazard(  EntityID& entity, int hitboxID, bool setFlag = true);
//
//
//
//void SetCollisionEvent(EntityID& entity, int hitboxID, void* function);
//
///// <summary>
///// Create a convex shape with corners relative to the position of the entity, then offset.
///// </summary>
///// <param name="entity"></param>
///// <param name="offsetX"></param>
///// <param name="offsetZ"></param>
///// <param name="corners">Max size of CONVEX_CORNER_LIMIT, currently 8</param>
///// <param name="cornerPosX"></param>
///// <param name="cornerPosY"></param>
///// <returns></returns>
//int CreateHitbox(EntityID& entity, int corners, float cornerPosX[], float cornerPosZ[]);

void AddHitboxComponent(EntityID& entity);

void CreateProximityHitbox(EntityID& entity, std::string fileName = "default"); //File name without extension ending

void AddProximityHitboxComponent(EntityID& entity);

void RenderGeometryIndependentCollisionToTexture(EntityID& stageEntity);

