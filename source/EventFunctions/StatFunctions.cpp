#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"

void SetSpeedZero(EntityID& entity, const int& index)
{
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	stats->baseMoveSpeed = stats->moveSpeed;
	stats->moveSpeed = 0.0f;
}
void ResetSpeed(EntityID& entity, const int& index)
{
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	stats->moveSpeed = stats->baseMoveSpeed;
}