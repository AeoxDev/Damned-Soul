// Come with me if you want the variable
#include <random>
#include "DeltaTime.h"
#include "Skynet\BehaviourHelper.h"
#include <DirectXMath.h>
#include "Registry.h"

float Calculate2dDistance(float pos1X, float pos1Z, float pos2X, float pos2Z)
{
	return sqrt((pos1X - pos2X) * (pos1X - pos2X) + (pos1Z - pos2Z) * (pos1Z - pos2Z));
}

void SmoothRotation(TransformComponent* tc, float goalX, float goalZ, float rotationFactor)
{
	DirectX::XMVECTOR goalV = DirectX::XMVECTOR{ goalX, goalZ, 0.0f };
	DirectX::XMVECTOR length = DirectX::XMVector3Length(goalV);
	DirectX::XMFLOAT3 l;
	DirectX::XMStoreFloat3(&l, length);
	float angle = acosf(tc->facingX);

	if (rotationFactor <= 1.0f)
	{
		rotationFactor = 1.1f;
	}

	if (tc->facingZ < 0.0f)
	{
		angle *= -1.0f;
	}

	if (l.x > 0.001f)
	{
		goalV = DirectX::XMVector2Normalize(goalV);

		DirectX::XMFLOAT3 goalFloats;
		DirectX::XMStoreFloat3(&goalFloats, goalV);
		goalX = goalFloats.x;
		goalZ = goalFloats.y;
		float goalAngle = acosf(goalX);
		if (goalZ < 0.0f)
		{
			goalAngle *= -1.0f;
		}
		//Check if shortest distance is right or left
		float orthogonalX = -tc->facingZ;
		float orthogonalZ = tc->facingX;
		float dot = goalX * tc->facingX + goalZ * tc->facingZ;
		float orthDot = goalX * orthogonalX + goalZ * orthogonalZ;
		if (orthDot > 0.0f)
		{//if left
			angle += GetDeltaTime() * (1.01f - dot) * rotationFactor;
		}
		else
		{
			angle -= GetDeltaTime() * (1.01f - dot) * rotationFactor;
		}
		tc->facingX = cosf(angle);
		tc->facingZ = sinf(angle);
	}
}

void TransformDecelerate(EntityID& entity)
{
	StatComponent* stat = registry.GetComponent<StatComponent>(entity);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	transform->currentSpeedX -= stat->m_acceleration * transform->currentSpeedX * GetDeltaTime();
	transform->currentSpeedZ -= stat->m_acceleration * transform->currentSpeedZ * GetDeltaTime();
}

void TransformAccelerate(EntityID& entity, float x, float z)
{
	StatComponent* stat = registry.GetComponent<StatComponent>(entity);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	float len = transform->currentSpeedX * transform->currentSpeedX + transform->currentSpeedZ * transform->currentSpeedZ;
	if (len > (stat->GetSpeed() * stat->GetSpeed()))
	{
		//len = sqrtf(len);
		//transform->currentSpeedX *= stat->GetSpeed() / len;//Get down to movementspeed instead.
		//transform->currentSpeedZ *= stat->GetSpeed() / len;
		TransformDecelerate(entity);
	}
	transform->currentSpeedX += stat->m_acceleration * x * stat->GetSpeed() * GetDeltaTime();
	transform->currentSpeedZ += stat->m_acceleration * z * stat->GetSpeed() * GetDeltaTime();
}
