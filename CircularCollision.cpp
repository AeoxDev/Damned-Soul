#include "CircularCollision.h"

bool IsCircularCollision(const Entity& entity1, const Entity& entity2)
{
	// Needs to loop through entities in case of several hitboxes

	float dx = entity1.components[CIRCLE_COMPONENT_INDEX].posX - entity2.components[CIRCLE_COMPONENT_INDEX].posX;
	float dy = entity1.components[CIRCLE_COMPONENT_INDEX].posY - entity2.components[CIRCLE_COMPONENT_INDEX].posY;

	float distance = std::sqrt(dx * dx + dy * dy);

	return distance <= (entity1.radius + entity2.radius);
}

void CollisionEvent(Entity& entity, Entity& entity2)
{
	// find on collision component in each entity

	//extract positions from each entity 
	//find last positions from each entitiy
	// this gives us the tools to calculate the angle of attack

	// calls on OnCollision
}




