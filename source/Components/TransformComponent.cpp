#include "TransformComponent.h"
#include <cmath>

float DistanceBetweenTransforms(TransformComponent* f, TransformComponent* s)
{
	float a = f->positionX - s->positionX;
	float b = f->positionY - s->positionY;
	float c = f->positionZ - s->positionZ;
	return sqrt(a * a + b * b + c * c);
}

TransformComponent::TransformComponent(TransformComponent* comp)
{
	memcpy(this, comp, sizeof(TransformComponent));
}
