#include "EventFunctions.h"
#include "TimedEventComponent.h"
#include "Components.h"
#include "Registry.h"
#include <cmath> //sin

void PauseAnimation(EntityID& entity, const int& index)
{
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	if (anim != nullptr)
	{
		anim->aAnimTimeFactor = 0.0f;
	}
}

void ContinueAnimation(EntityID& entity, const int& index)
{
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	if (anim != nullptr)
	{
		anim->aAnimTimeFactor = 1.0f;
	}
}

void ResetAnimation(EntityID& entity, const int& index)
{
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	if (anim != nullptr)
	{
		anim->aAnimTime = 0.0f;
	}
	
}

void BlinkColor(EntityID& entity, const int& index)
{
	//Flash color red repeatedly
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);
	float frequency = 10.0f; //Higher frequency = faster flashing lights
	float cosineWave = std::cosf(GetEventTimedElapsed(entity, index) * frequency) * std::cosf(GetEventTimedElapsed(entity, index) * frequency);
	if (skelel)
		skelel->colorAdditiveRed = cosineWave;
	if (bonel)
		bonel->colorAdditiveRed = cosineWave;

}

void ResetColor(EntityID& entity, const int& index)
{
	//Flash color red repeatedly
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);
	float frequency = 10.0f; //Higher frequency = faster flashing lights
	float cosineWave = std::cosf(GetEventTimedElapsed(entity, index) * frequency) * std::cosf(GetEventTimedElapsed(entity, index) * frequency);
	if (skelel)
		skelel->colorAdditiveRed = 0.0f;
	if (bonel)
		bonel->colorAdditiveRed = 0.0f;

}