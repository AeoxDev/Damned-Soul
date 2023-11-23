#include "MouseComponent.h"
#include <DirectXMath.h>
#include "Camera.h"
#include "Input.h"
#include "SDLHandler.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "Registry.h"
using namespace DirectX;

struct MouseComponent
{
	XMMATRIX inversePerspective;
	XMMATRIX inverseOrthographic;
	XMMATRIX inverseView;
	float directionX, directionZ;//Direction from the entitites transform to the mouse (facing direction)
};

void MouseComponentAddComponent(EntityID entityID)
{
	MouseComponent* mouseComponent = registry.AddComponent<MouseComponent>(entityID);
	MouseComponentUpdateDirection(entityID);
}

void MouseComponentUpdateDirection(EntityID entityID,const bool& isPerspective)
{

	//First get where on screen the mouse is in screenspace.
	
	float mousePosX = (float)mouseX / (float)sdl.WIDTH;//Down to 0.0f to 1.0f
	mousePosX = (mousePosX - 0.5f) * 2.0f;//From -1.0f to 1.0f;
	float mousePosY = (float)mouseY / (float)sdl.HEIGHT;
	mousePosY = (mousePosY - 0.5f) * 2.0f;
	//Then get the position of the transformComponent. Take the transformComponent to the screenspace.
	TransformComponent* transformComponent = registry.GetComponent<TransformComponent>(entityID);
	assert(transformComponent != nullptr);//The mouse requires a transformComponent in order to update its direction!
	XMVECTOR toScreenSpace = XMVECTOR{ transformComponent->positionX,transformComponent->positionY, transformComponent->positionZ};
	toScreenSpace = XMVector3Transform(toScreenSpace,Camera::GetView());
	toScreenSpace = XMVector3Transform(toScreenSpace,Camera::GetPerspective());
	MouseComponent* mouseComponent = registry.GetComponent<MouseComponent>(entityID);
	assert(transformComponent != nullptr);//The mouse requires its component to update its direction!
	XMFLOAT3 toFloat;
	XMStoreFloat3(&toFloat, toScreenSpace);
	mouseComponent->directionX = mousePosX - toFloat.x / toFloat.z;
	mouseComponent->directionZ = mousePosY - toFloat.y / toFloat.z;
	float len = sqrtf(mouseComponent->directionX * mouseComponent->directionX + mouseComponent->directionZ * mouseComponent->directionZ);
	mouseComponent->directionX /= len;
	mouseComponent->directionZ /= -len;
}

float MouseComponentGetDirectionX(MouseComponent*& mouseComponent)
{
	return mouseComponent->directionX;
}

float MouseComponentGetDirectionZ(MouseComponent*& mouseComponent)
{
	return mouseComponent->directionZ;
}
