#include "Systems\Systems.h"
#include "Camera.h"
#include "Components.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"

bool PointOfInterestSystem::Update()
{
	PointOfInterestComponent* poiCo = nullptr;
	TransformComponent* tCo = nullptr;
	DirectX::XMVECTOR lookVector = Camera::GetLookAt();
	DirectX::XMFLOAT3 lookPos;
	DirectX::XMStoreFloat3(&lookPos, lookVector);
	//Use this as a proxy for current position
	float cameraPosX = lookPos.x;
	float cameraPosY = lookPos.y;
	float cameraPosZ = lookPos.z;
	float newPosX = 0;
	float newPosY = 0;
	float newPosZ = 0;
	float points = 1;//The 1 is the camera itself
	//DirectX::XMStoreFloat3(&cameraPos, cameraVPos);
	for (auto entity : View<PointOfInterestComponent, TransformComponent>(registry))
	{
		//Get POI data if needed
		poiCo = registry.GetComponent<PointOfInterestComponent>(entity);

		//Get position
		tCo = registry.GetComponent<TransformComponent>(entity);
		if (poiCo->m_mode == POI_ACTIVE)
		{
			//First add the new positions, use the last position aswell for a smoother movement (midpoint of both positions)
			newPosX += (tCo->positionX + tCo->lastPositionX) * poiCo->m_weight;
			newPosY += (tCo->positionY + tCo->lastPositionY) * poiCo->m_weight;
			newPosZ += (tCo->positionZ + tCo->lastPositionZ) * poiCo->m_weight;
			points += 2.0f * poiCo->m_weight;
		}
		else if (poiCo->m_mode == POI_ACTIVE_FOR_X_TIME)
		{
			poiCo->m_time -= GetDeltaTime();
			//First add the new positions, use the last position aswell for a smoother movement (midpoint of both positions)
			newPosX += (tCo->positionX + tCo->lastPositionX) * poiCo->m_weight;
			newPosY += (tCo->positionY + tCo->lastPositionY) * poiCo->m_weight;
			newPosZ += (tCo->positionZ + tCo->lastPositionZ) * poiCo->m_weight;
			//Multiply by weights
			points += 2.0f * poiCo->m_weight;
			if (poiCo->m_time < 0.0f)
			{
				poiCo->m_mode = POI_INACTIVE;
			}
		}
		else if (poiCo->m_mode == POI_INACTIVE_FOR_X_TIME)
		{
			poiCo->m_time -= GetDeltaTime();
			if (poiCo->m_time < 0.0f)
			{
				poiCo->m_mode = POI_ACTIVE;
			}
		}
		else if (poiCo->m_mode == POI_FORCE)
		{
			Camera::SetPosition(tCo->positionX, tCo->positionY, tCo->positionZ, true);
			Camera::SetLookAt(tCo->positionX, tCo->positionY, tCo->positionZ);
			Camera::UpdateView();
			Camera::UpdateProjection();
			return true;
		}
		
	}
	float avX = (cameraPosX + newPosX) / (float)points;
	float avY = (cameraPosY + newPosY) / (float)points;
	float avZ = (cameraPosZ + newPosZ) / (float)points;
	PlayerComponent* p = nullptr;
	TransformComponent* pt = nullptr;
	//Offset forward by player's movement:
	//Only supports one player at the moment
	for (auto entity : View<PointOfInterestComponent, TransformComponent, PlayerComponent>(registry))
	{
		 p = registry.GetComponent<PlayerComponent>(entity);
		 pt = registry.GetComponent<TransformComponent>(entity);
		
	}
	if (p == nullptr || pt == nullptr)
	{
		return true;
	}
	float playerMoveX = pt->facingX;
	float playerMoveY = pt->facingY;
	float playerMoveZ = pt->facingZ;
	float moveScalar = p->movementSpeed * p->moveTime;

	float distX = avX - cameraPosX;
	float distY = avY - cameraPosY;
	float distZ = avZ - cameraPosZ;

	//Calculate max distance from everypoint.
	float maxDistance = -100000000.0f;
	for (auto entity : View<PointOfInterestComponent, TransformComponent>(registry))
	{
		//Get POI data if needed
		poiCo = registry.GetComponent<PointOfInterestComponent>(entity);

		//Get position
		tCo = registry.GetComponent<TransformComponent>(entity);
		float distSquared = (tCo->positionX - avX) * (tCo->positionX - avX) + (tCo->positionZ - avZ) * (tCo->positionZ - avZ);
		if (maxDistance < distSquared)
		{
			maxDistance = distSquared;
		}
	}
	float lastFOV = Camera::GetFOV();
	Camera::SetFOV((powf(maxDistance, 0.375f)) * 0.015f);
	float newFOV = Camera::GetFOV();
	float fovDist = newFOV - lastFOV;
	float fovScalar = GetDeltaTime();
	Camera::SetFOV(lastFOV + fovDist * fovScalar);
	float distanceFactor = 5.0f;

	distX *= distanceFactor;
	distY *= distanceFactor;
	distZ *= distanceFactor;

	float scalar = GetDeltaTime() * 0.8f;
	float posX = cameraPosX + (distX + playerMoveX * moveScalar)*scalar;
	float posY = cameraPosY + (distY + playerMoveY * moveScalar)*scalar;
	float posZ = cameraPosZ + (distZ + playerMoveZ * moveScalar)*scalar;

	Camera::SetPosition(posX, posY, posZ, true);
	Camera::SetLookAt(posX, posY, posZ);
	Camera::UpdateView();
	Camera::UpdateProjection();
	//Camera::UpdateProjection();//Do when zoom in out
	return true;
}