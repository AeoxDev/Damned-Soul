#include "Systems\Systems.h"
#include "Camera.h"
#include "Components.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "SDLHandler.h"
#include "Input.h"

#define CAMERA_MOVESPEED 1.5f
#define CAMERA_ZOOMSPEED 1.75f
#define CAMERA_PREDICT_FACTOR 0.0f//Predict x seconds ahead
#define CAMERA_MOUSE_FACTOR 0.15f

bool PointOfInterestSystem::Update()
{
	if (gameSpeed == 0.0f)
		return true;

	//DEBUG Controls
#ifdef _DEBUG
	if (keyInput[SCANCODE_UP] == down)
	{
		Camera::AdjustRotation(-1.0f * GetDeltaTime(), 0.0f, 0.0f);
	}
	if (keyInput[SCANCODE_DOWN] == down)
	{
		Camera::AdjustRotation(1.0f * GetDeltaTime(), 0.0f, 0.0f);
	}
#endif // _DEBUG

	

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
		if (poiCo->mode == POI_ACTIVE)
		{
			float predict = CAMERA_PREDICT_FACTOR / (GetDeltaTime() + 0.0001f);
			float difX = (tCo->positionX - tCo->lastPositionX) * predict;//From last to position
			float difY = (tCo->positionY - tCo->lastPositionY) * predict;//From last to position
			float difZ = (tCo->positionZ - tCo->lastPositionZ) * predict;//From last to position
			float dist = (difX * difX + difY * difY + difZ * difZ);
			if (dist > 1000.0f)
			{
				difX = 0.0f;
				difY = 0.0f;
				difZ = 0.0f;
			}
			//First add the new positions, use the last position aswell for a smoother movement (midpoint of both positions)
			newPosX += (tCo->positionX + difX) * poiCo->weight;
			newPosY += (tCo->positionY + difY) * poiCo->weight;
			newPosZ += (tCo->positionZ + difZ) * poiCo->weight;
			points += poiCo->weight;
		}
		else if (poiCo->mode == POI_MOUSE)
		{
			float xOffset = (float)mouseX - (float)sdl.WIDTH  * 0.5f;
			float zOffset = (float)mouseY - (float)sdl.HEIGHT * 0.5f;
			newPosX += xOffset * CAMERA_MOUSE_FACTOR;
			newPosZ += -zOffset *  CAMERA_MOUSE_FACTOR;
			points += 1.0f;
		}
		else if (poiCo->mode == POI_ACTIVE_FOR_X_TIME)
		{
			poiCo->time -= GetDeltaTime();
			//First add the new positions, use the last position aswell for a smoother movement (midpoint of both positions)
			float predict = CAMERA_PREDICT_FACTOR / (GetDeltaTime() + 0.0001f);
			float difX = (tCo->positionX - tCo->lastPositionX) * predict;//From last to position
			float difY = (tCo->positionY - tCo->lastPositionY) * predict;//From last to position
			float difZ = (tCo->positionZ - tCo->lastPositionZ) * predict;//From last to positionif (difX * difX + difY*difY + difZ*difZ > 4.0f)
			{
				difX = 0.0f;
				difY = 0.0f;
				difZ = 0.0f;
			}
			newPosX += (tCo->positionX + difX) * poiCo->weight;
			newPosY += (tCo->positionY + difX) * poiCo->weight;
			newPosZ += (tCo->positionZ + difX) * poiCo->weight;
			//Multiply by weights
			points += 2.0f * poiCo->weight;
			if (poiCo->time < 0.0f)
			{
				poiCo->mode = POI_INACTIVE;
			}
		}
		else if (poiCo->mode == POI_INACTIVE_FOR_X_TIME)
		{
			poiCo->time -= GetDeltaTime();
			if (poiCo->time < 0.0f)
			{
				poiCo->mode = POI_ACTIVE;
			}
		}
		else if (poiCo->mode == POI_FORCE)
		{
			poiCo->rotationY += GetDeltaTime() * poiCo->rotationAccel;
			//(0.0, -200) -> (200, 0.0) -> (0.0, 200) -> (-200, 0.0)
			Camera::SetPosition(tCo->positionX + poiCo->rotationRadius * -sinf(poiCo->rotationY), tCo->positionY + poiCo->height + CAMERA_OFFSET_Y, tCo->positionZ + poiCo->rotationRadius * cosf(poiCo->rotationY), false);
			Camera::SetLookAt(tCo->positionX, tCo->positionY, tCo->positionZ);
			Camera::UpdateView();
			Camera::UpdateProjection();
			return true;
		}
		
		
	}
	float avX = (cameraPosX + newPosX) / (float)points;
	float avY = (cameraPosY + newPosY) / (float)points;
	float avZ = (cameraPosZ + newPosZ) / (float)points;

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
		if (poiCo->mode == POI_ACTIVE || poiCo->mode == POI_ACTIVE_FOR_X_TIME)
		{
			float distSquared = (tCo->positionX - avX) * (tCo->positionX - avX) + (tCo->positionZ - avZ) * (tCo->positionZ - avZ);
			if (maxDistance < distSquared)
			{
				maxDistance = distSquared;
			}
		}
		
	}
	float lastFOV = Camera::GetFOV();
	Camera::SetFOV((powf(maxDistance, 0.375f)) * 0.015f);
	float newFOV = Camera::GetFOV();
	float fovDist = newFOV - lastFOV;
	float fovScalar = GetDeltaTime() * CAMERA_ZOOMSPEED;
	Camera::SetFOV(lastFOV + fovDist * fovScalar);
	float distanceFactor = 5.0f;

	distX *= distanceFactor;
	distY *= distanceFactor;
	distZ *= distanceFactor;

	float scalar = GetDeltaTime() * CAMERA_MOVESPEED;
	float posX = cameraPosX + (distX)*scalar;
	float posY = cameraPosY + (distY)*scalar;
	float posZ = cameraPosZ + (distZ)*scalar;

	Camera::SetPosition(posX, posY, posZ, true);
	Camera::SetLookAt(posX, posY, posZ);
	Camera::UpdateView();
	Camera::UpdateProjection();
	//Camera::UpdateProjection();//Do when zoom in out
	return true;
}