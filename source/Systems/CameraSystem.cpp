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
	int points = 1;
	//DirectX::XMStoreFloat3(&cameraPos, cameraVPos);
	for (auto entity : View<PointOfInterestComponent, TransformComponent>(registry))
	{
		//Get POI data if needed
		poiCo = registry.GetComponent<PointOfInterestComponent>(entity);

		//Get position
		tCo = registry.GetComponent<TransformComponent>(entity);
		newPosX += tCo->positionX + tCo->lastPositionX;
		newPosY += tCo->positionY + tCo->lastPositionY;
		newPosZ += tCo->positionZ + tCo->lastPositionZ;
		points += 2;
	}
	float avX = (cameraPosX + newPosX) / (float)points;
	float avY = (cameraPosY + newPosY) / (float)points;
	float avZ = (cameraPosZ + newPosZ) / (float)points;
	ControllerComponent* c = nullptr;
	TransformComponent* pt = nullptr;
	StatComponent* s = nullptr;
	//Offset forward by player's movement:
	//Only supports one player at the moment
	for (auto entity : View<PointOfInterestComponent, TransformComponent, ControllerComponent, StatComponent>(registry))
	{
		 s = registry.GetComponent<StatComponent>(entity);
		 c = registry.GetComponent<ControllerComponent>(entity);
		 pt = registry.GetComponent<TransformComponent>(entity);
		
	}
	if (c == nullptr || pt == nullptr)
	{
		return true;
	}
	float playerMoveX = pt->facingX;
	float playerMoveY = pt->facingY;
	float playerMoveZ = pt->facingZ;
	float moveScalar = s->moveSpeed * c->moveTime;

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
	Camera::SetFOV((sqrtf(maxDistance)) * 0.01f);
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