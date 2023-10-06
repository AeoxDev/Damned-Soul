#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"

bool ControllerSystem::Update()
{
	for (auto entity : View<ControllerComponent, TransformComponent, StatComponent>(registry))
	{
		//Get the relevant components from the entity
		ControllerComponent* controller = registry.GetComponent<ControllerComponent>(entity);
		StatComponent* stat = registry.GetComponent<StatComponent>(entity);
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);

		//Store variables for checking to see how far the entity has moved, these are relevant to the camera
		transform->lastPositionZ = transform->positionZ;
		transform->lastPositionX = transform->positionX;
		DirectX::XMVECTOR goalV = DirectX::XMVECTOR{ controller->goalX,controller->goalZ, 0.0f };
		DirectX::XMVECTOR length = DirectX::XMVector3Length(goalV);
		DirectX::XMFLOAT3 l;
		DirectX::XMStoreFloat3(&l, length);
		float angle = acosf(transform->facingX);

		//Calculations for the system of camera following points of interest etc
		if (transform->facingZ < 0.0f)
		{
			angle *= -1.0f;
		}
		if (l.x > 0.001f)
		{
			goalV = DirectX::XMVector2Normalize(goalV);

			DirectX::XMFLOAT3 goalFloats;
			DirectX::XMStoreFloat3(&goalFloats, goalV);
			controller->goalX = goalFloats.x;
			controller->goalZ = goalFloats.y;
			float goalAngle = acosf(controller->goalX);
			if (controller->goalZ < 0.0f)
			{
				goalAngle *= -1.0f;
			}
			//Check if shortest distance is right or left
			float orthogonalX = -transform->facingZ;
			float orthogonalZ = transform->facingX;
			//float dot = playerInputs[i].aimDirection.Dot(players[i].forward);
			float dot = controller->goalX * transform->facingX + controller->goalZ * transform->facingZ;
			float orthDot = controller->goalX * orthogonalX + controller->goalZ * orthogonalZ;
			if (orthDot > 0.0f)
			{//Om till vänster
				angle += GetDeltaTime() * (10.1f - dot);
			}
			else
			{
				angle -= GetDeltaTime() * (10.1f - dot);
			}
			transform->facingX = cosf(angle);
			transform->facingZ = sinf(angle);
			controller->goalX = 0.0f;
			controller->goalZ = 0.0f;

		}
		//End of: Camera System thing

		/*COMBAT INPUT*/
		//Test code for now but we fuckin about
		float speed = stat->moveSpeed;
		if (keyInput[SCANCODE_LSHIFT] == down)
		{
			speed *= 3.0f;
		}

		/*MOVEMENT INPUT*/
		bool moving = false;
		if (keyInput[SCANCODE_W] == down)
		{
			moving = true;
			
			transform->positionZ += speed * GetDeltaTime();
			controller->goalZ += 1.0f;
		}
		if (keyInput[SCANCODE_S] == down)
		{
			moving = true;
			transform->positionZ -= speed * GetDeltaTime();
			controller->goalZ -= 1.0f;
		}
		if (keyInput[SCANCODE_A] == down)
		{
			moving = true;
			transform->positionX -= speed * GetDeltaTime();
			controller->goalX -= 1.0f;
		}
		if (keyInput[SCANCODE_D] == down)
		{
			moving = true;
			transform->positionX += speed * GetDeltaTime();
			controller->goalX += 1.0f;
		}

		if (moving)
		{
			controller->moveTime += GetDeltaTime() * controller->moveFactor;
			if (controller->moveMaxLimit < controller->moveTime) //clamp moveTime to upper limit
			{
				controller->moveTime = controller->moveMaxLimit;
			}
		}
		else
		{
			controller->moveTime -= GetDeltaTime() * controller->moveFactor * controller->moveResetFactor;
			if (controller->moveTime < 0.0f) //clamp moveTime to lower limit
			{
				controller->moveTime = 0.0f;
			}
		}
	}
		
	return true;
}