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
	float cosineWave = std::cosf(GetTimedEventElapsedTime(entity, index) * frequency) * std::cosf(GetTimedEventElapsedTime(entity, index) * frequency);
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
	float cosineWave = std::cosf(GetTimedEventElapsedTime(entity, index) * frequency) * std::cosf(GetTimedEventElapsedTime(entity, index) * frequency);
	if (skelel)
		skelel->colorAdditiveRed = 0.0f;
	if (bonel)
		bonel->colorAdditiveRed = 0.0f;

}

void SquashStretch(EntityID& entity, const int& index)
{
	//Use scaling for squash stretch, remember last scaling
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	if (!transform)
	{
		return;
	}
	SquashStretchComponent* squashStrech = registry.GetComponent<SquashStretchComponent>(entity);
	assert(squashStrech != nullptr);//This timed event requires a SquashStrechComponent to function correctly!
	if (!squashStrech)
	{
		return;
	}
	//Do the scaling here

	if (squashStrech->type == SquashStretchType::Constant)
	{
		//Ignore time.
		transform->offsetScaleX = squashStrech->goalScaleX;
		transform->offsetScaleY = squashStrech->goalScaleY;
		transform->offsetScaleZ = squashStrech->goalScaleZ;
		return;
	}

	float currentTime = GetTimedEventElapsedTime(entity, index);
	float totalTime = GetTimedEventTotalTime(entity, index);
	if (squashStrech->type == Linear)
	{
		//Start at start if available, otherwise transform scale.
		float scaleX = transform->scaleX;
		float scaleY = transform->scaleY;
		float scaleZ = transform->scaleZ;
		if (squashStrech->useStartScale)
		{
			scaleX = squashStrech->startScaleX;
			scaleY = squashStrech->startScaleY;
			scaleZ = squashStrech->startScaleZ;
		}
		float linearTime = (currentTime / (totalTime + 0.00001f));
		float dx = squashStrech->goalScaleX - scaleX;
		float dy = squashStrech->goalScaleY - scaleY;
		float dz = squashStrech->goalScaleZ - scaleZ;
		transform->offsetScaleX = scaleX + dx * linearTime;
		transform->offsetScaleY = scaleY + dy * linearTime;
		transform->offsetScaleZ = scaleZ + dz * linearTime;
	}
}

void ResetSquashStretch(EntityID& entity, const int& index)
{
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	if (!transform)
	{
		return;
	}
	transform->offsetScaleX = 1.0f;
	transform->offsetScaleY = 1.0f;
	transform->offsetScaleZ = 1.0f;
}
