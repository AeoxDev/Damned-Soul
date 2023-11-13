#include "EventFunctions.h"
#include "TimedEventComponent.h"
#include "Components.h"
#include "Registry.h"
#include "Camera.h"
#include <assert.h>


void CutsceneCreateLinearTransition(EntityID& entity, const int& index)
{
	float transitionTime = GetTimedEventTotalTime(entity, index);
	CutsceneComponent* cutscene = registry.AddComponent<CutsceneComponent>(entity);
	DirectX::XMFLOAT3 lookAt = Camera::GetLookAtFloat();
	cutscene->startLookAtX = lookAt.x;
	cutscene->startLookAtY = lookAt.y;
	cutscene->startLookAtZ = lookAt.z;

	DirectX::XMFLOAT3 position = Camera::GetPositionFloat();
	cutscene->startPositionX = position.x;
	cutscene->startPositionY = position.y;
	cutscene->startPositionZ = position.z;
	 
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	assert(transform != nullptr);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY;
	cutscene->goalLookAtZ = transform->positionZ;

	cutscene->goalPositionX = transform->positionX + CAMERA_OFFSET_X * 0.1f;
	cutscene->goalPositionY = transform->positionY + CAMERA_OFFSET_Y * 0.1f;
	cutscene->goalPositionZ = transform->positionZ + CAMERA_OFFSET_Z * 0.1f;
	


	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, transitionTime, EndCutscene);
}
void BeginCutscene(EntityID& entity, const int& index)
{
	Camera::SetCutsceneMode(true);
}
void EndCutscene(EntityID& entity, const int& index)
{
	Camera::SetCutsceneMode(false);
}
//Do the given cutscene components arguments over time
void CutsceneTransition(EntityID& entity, const int& index)
{
	CutsceneComponent* cutscene = registry.GetComponent<CutsceneComponent>(entity);
	assert(cutscene != nullptr);
	float time = GetTimedEventElapsedTime(entity, index);
	float totalTime = GetTimedEventTotalTime(entity, index);
	float scalar = time / totalTime;//From 0 to 1

	

	if (cutscene->mode | Cutscene_Linear)
	{
		
		if (cutscene->mode | CutsceneMode::Transition_Position)
		{
			float posDifX = cutscene->goalPositionX - cutscene->startPositionX;
			float posDifY = cutscene->goalPositionY - cutscene->startPositionY;
			float posDifZ = cutscene->goalPositionZ - cutscene->startPositionZ;
			float newPosX = posDifX * scalar + cutscene->startPositionX;
			float newPosY = posDifY * scalar + cutscene->startPositionY;
			float newPosZ = posDifZ * scalar + cutscene->startPositionZ;
			Camera::SetPosition(newPosX, newPosY, newPosZ, false);
		}
		if (cutscene->mode | CutsceneMode::Transition_LookAt)
		{
			float lookAtDifX = cutscene->goalLookAtX - cutscene->startLookAtX;
			float lookAtDifY = cutscene->goalLookAtY - cutscene->startLookAtY;
			float lookAtDifZ = cutscene->goalLookAtZ - cutscene->startLookAtZ;
			float newLookAtX = lookAtDifX * scalar + cutscene->startLookAtX;
			float newLookAtY = lookAtDifY * scalar + cutscene->startLookAtY;
			float newLookAtZ = lookAtDifZ * scalar + cutscene->startLookAtZ;
			Camera::SetLookAt(newLookAtX, newLookAtY, newLookAtZ);
		}
	}
	
}

