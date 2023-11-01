#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"

void SetSpeedZero(EntityID& entity, const int& index)
{
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	stats->SetSpeedMult(0.f);
}
void ResetSpeed(EntityID& entity, const int& index)
{
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	stats->SetSpeedMult(1.f);
}