#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"

bool ControllerSystem::Update()
{
	for (auto entity : View<PlayerComponent, TransformComponent, StatComponent>(registry))
	{
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
		StatComponent* stat = registry.GetComponent<StatComponent>(entity);
		TransformComponent* pt = registry.GetComponent<TransformComponent>(entity);
		pt->lastPositionZ = pt->positionZ;
		pt->lastPositionX = pt->positionX;
		DirectX::XMVECTOR goalV = DirectX::XMVECTOR{ player->goalX,player->goalZ, 0.0f };
		DirectX::XMVECTOR length = DirectX::XMVector3Length(goalV);
		DirectX::XMFLOAT3 l;
		DirectX::XMStoreFloat3(&l, length);
		float angle = acosf(pt->facingX);
		if (pt->facingZ < 0.0f)
		{
			angle *= -1.0f;
		}
		if (l.x > 0.001f)
		{
			goalV = DirectX::XMVector2Normalize(goalV);

			DirectX::XMFLOAT3 goalFloats;
			DirectX::XMStoreFloat3(&goalFloats, goalV);
			player->goalX = goalFloats.x;
			player->goalZ = goalFloats.y;
			float goalAngle = acosf(player->goalX);
			if (player->goalZ < 0.0f)
			{
				goalAngle *= -1.0f;
			}
			//Check if shortest distance is right or left
			float orthogonalX = -pt->facingZ;
			float orthogonalZ = pt->facingX;
			//float dot = playerInputs[i].aimDirection.Dot(players[i].forward);
			float dot = player->goalX * pt->facingX + player->goalZ * pt->facingZ;
			float orthDot = player->goalX * orthogonalX + player->goalZ * orthogonalZ;
			if (orthDot > 0.0f)
			{//Om till vänster
				angle += GetDeltaTime() * (10.1f - dot);
			}
			else
			{
				angle -= GetDeltaTime() * (10.1f - dot);
			}
			pt->facingX = cosf(angle);
			pt->facingZ = sinf(angle);
			player->goalX = 0.0f;
			player->goalZ = 0.0f;

		}
		bool moving = false;
		if (keyInput[SCANCODE_W] == down)
		{
			moving = true;
			
			pt->positionZ += stat->moveSpeed * GetDeltaTime();
			player->goalZ += 1.0f;
		}
		if (keyInput[SCANCODE_S] == down)
		{
			moving = true;
			pt->positionZ -= stat->moveSpeed * GetDeltaTime();
			player->goalZ -= 1.0f;
		}
		if (keyInput[SCANCODE_A] == down)
		{
			moving = true;
			pt->positionX -= stat->moveSpeed * GetDeltaTime();
			player->goalX -= 1.0f;
		}
		if (keyInput[SCANCODE_D] == down)
		{
			moving = true;
			pt->positionX += stat->moveSpeed * GetDeltaTime();
			player->goalX += 1.0f;
		}
		if (moving)
		{
			player->moveTime += GetDeltaTime() * player->moveFactor;
			if (player->moveMaxLimit < player->moveTime)
			{
				player->moveTime = player->moveMaxLimit;
			}
		}
		else
		{
			player->moveTime -= GetDeltaTime() * player->moveFactor * player->moveResetFactor;
			if (player->moveTime < 0.0f)
			{
				player->moveTime = 0.0f;
			}
		}
	}
	
		
		
	return true;
}