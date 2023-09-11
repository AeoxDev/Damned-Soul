#include "Backend\CircularCollision.h"
#include "Backend\Collision.h"
//bool IsCircularCollision()
//{
//	// Needs to loop through entities in case of several hitboxes
//
//	/*float dx = entity1.components[CIRCLE_COMPONENT_INDEX].posX - entity2.components[CIRCLE_COMPONENT_INDEX].posX;
//	float dy = entity1.components[CIRCLE_COMPONENT_INDEX].posY - entity2.components[CIRCLE_COMPONENT_INDEX].posY;
//
//	float distance = std::sqrt(dx * dx + dy * dy);
//
//	return distance <= (entity1.radius + entity2.radius);*/
//	return false;
//}

bool IsCircularCollision(const Registry& registry1, const Registry& registry2, const EntityID& entity1, const EntityID& entity2, int circle1, int circle2)
{
	// get a hold of hitbox components from entity
	

	// get a hold of both the circles' bit flags to check if they can collide or not

	// return false?

	// if not, find the hitboxes and see if they actually collide

	// return true/false
}
