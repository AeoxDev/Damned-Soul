#include "EventFunctions.h"
#include "TimedEventComponent.h"
#include "Components.h"
#include "Registry.h"
#include <cmath> //sin
#include <assert.h>

void PauseAnimation(EntityID& entity, const int& index)
{
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	BlendAnimationComponent* blendAnim = registry.GetComponent<BlendAnimationComponent>(entity);
	if (anim != nullptr)
	{
		anim->aAnimTimeFactor = 0.0f;
	}
	else if (blendAnim != nullptr)
	{
		blendAnim->anim1.aAnimTimeFactor = 0.0f;
		blendAnim->anim2.aAnimTimeFactor = 0.0f;
	}
}

void ContinueAnimation(EntityID& entity, const int& index)
{
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	BlendAnimationComponent* blendAnim = registry.GetComponent<BlendAnimationComponent>(entity);
	if (anim != nullptr)
	{
		anim->aAnimTimeFactor = 1.0f;
	}
	else if (blendAnim != nullptr)
	{
		blendAnim->anim1.aAnimTimeFactor = 1.0f;
		blendAnim->anim2.aAnimTimeFactor = 1.0f;
	}
}

void ResetAnimation(EntityID& entity, const int& index)
{
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	BlendAnimationComponent* blendAnim = registry.GetComponent<BlendAnimationComponent>(entity);
	if (anim != nullptr)
	{
		anim->aAnimTime = 0.0f;
	}
	else if (blendAnim != nullptr)
	{
		blendAnim->anim1.aAnimTime = 0.0f;
		blendAnim->anim2.aAnimTime = 0.0f;
	}
}

void BlinkColor(EntityID& entity, const int& index)
{
	//Flash color red repeatedly
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);
	float frequency = 10.0f; //Higher frequency = faster flashing lights
	float cosineWave = std::cosf(GetTimedEventElapsedTime(entity, index) * frequency);// *std::cosf(GetTimedEventElapsedTime(entity, index) * frequency);
	cosineWave *= cosineWave;
	if (skelel)
	{
		skelel->shared.bcmB_temp *= 1 - cosineWave;
		skelel->shared.bcmG_temp *= 1 - cosineWave;
		skelel->shared.bcaR_temp += cosineWave;
	}
	if (bonel)
	{
		bonel->shared.bcmB_temp *= 1 - cosineWave;
		bonel->shared.bcmG_temp *= 1 - cosineWave;
		bonel->shared.bcaR_temp += cosineWave;
	}

}

void ResetColor(EntityID& entity, const int& index)
{
	//Flash color red repeatedly
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);
	float frequency = 10.0f; //Higher frequency = faster flashing lights
	float cosineWave = std::cosf(GetTimedEventElapsedTime(entity, index) * frequency) * std::cosf(GetTimedEventElapsedTime(entity, index) * frequency);
	if (skelel)
	{
		skelel->shared.colorAdditiveRed = skelel->shared.baseColorAdditiveRed;
		skelel->shared.colorAdditiveGreen = skelel->shared.baseColorAdditiveGreen;
		skelel->shared.colorAdditiveBlue = skelel->shared.baseColorAdditiveBlue;

	}
	if (bonel)
	{
		bonel->shared.colorAdditiveRed = bonel->shared.baseColorAdditiveRed;
		bonel->shared.colorAdditiveGreen = bonel->shared.baseColorAdditiveGreen;
		bonel->shared.colorAdditiveBlue = bonel->shared.baseColorAdditiveBlue;
	}

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
	if (squashStrech->type == Accelerating)
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
		float acceleratingTime = (currentTime / (totalTime + 0.00001f));
		acceleratingTime *= acceleratingTime;
		float dx = squashStrech->goalScaleX - scaleX;
		float dy = squashStrech->goalScaleY - scaleY;
		float dz = squashStrech->goalScaleZ - scaleZ;
		transform->offsetScaleX = scaleX + dx * acceleratingTime;
		transform->offsetScaleY = scaleY + dy * acceleratingTime;
		transform->offsetScaleZ = scaleZ + dz * acceleratingTime;
	}
}
void TPose(EntityID& entity, const int& index)
{
	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	BlendAnimationComponent* blendAnim = registry.GetComponent<BlendAnimationComponent>(entity);
	if (anim != nullptr)
	{
		anim->aAnimIdx = 0;
		anim->aAnim = ANIMATION_DEATH;
		anim->aAnimTime = 0.01f;;
	}
	else if (blendAnim != nullptr)
	{
		blendAnim->anim1.aAnimIdx = 0;
		blendAnim->anim1.aAnim = ANIMATION_DEATH;
		blendAnim->anim1.aAnimTime = 0.01f;;

		blendAnim->anim2.aAnimIdx = 0;
		blendAnim->anim2.aAnim = ANIMATION_DEATH;
		blendAnim->anim2.aAnimTime = 0.01f;;
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

SquashStretchComponent* AddSquashStretch(EntityID& entity, const SquashStretchType type, const float& goalX, const float& goalY, const float& goalZ, const bool& useStartScale, const float& startX, const float& startY, const float& startZ)
{
	SquashStretchComponent* component = registry.AddComponent<SquashStretchComponent>(entity);
	component->type = type;
	component->goalScaleX = goalX;
	component->goalScaleY = goalY;
	component->goalScaleZ = goalZ;
	component->useStartScale = useStartScale;
	component->startScaleX = startX;
	component->startScaleY = startY;
	component->startScaleZ = startZ;
	return component;
}

void LoopFallingAnimation(EntityID& entity, const int& index)
{

}
void FallingAnimation(EntityID& entity, const int& index)
{

}