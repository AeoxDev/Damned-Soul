// Come with me if you want the variable
#include <random>
#include "DeltaTime.h"
#include "Skynet\BehaviourHelper.h"



float Calculate2dDistance(float pos1X, float pos1Z, float pos2X, float pos2Z)
{
	return sqrt((pos1X - pos2X) * (pos1X - pos2X) + (pos1Z - pos2Z) * (pos1Z - pos2Z));
}

void SmoothRotation(TransformComponent* ptc, float goalX, float goalZ, float rotationFactor)
{
	DirectX::XMVECTOR goalV = DirectX::XMVECTOR{ goalX, goalZ, 0.0f };
	DirectX::XMVECTOR length = DirectX::XMVector3Length(goalV);
	DirectX::XMFLOAT3 l;
	DirectX::XMStoreFloat3(&l, length);
	float angle = acosf(ptc->facingX);

	if (rotationFactor <= 1.0f)
	{
		rotationFactor = 1.1f;
	}

	if (ptc->facingZ < 0.0f)
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
		float orthogonalX = -ptc->facingZ;
		float orthogonalZ = ptc->facingX;
		float dot = goalX * ptc->facingX + goalZ * ptc->facingZ;
		float orthDot = goalX * orthogonalX + goalZ * orthogonalZ;
		if (orthDot > 0.0f)
		{//Om till vänster
			angle += GetDeltaTime() * (rotationFactor - dot);
		}
		else
		{
			angle -= GetDeltaTime() * (rotationFactor - dot);
		}
		ptc->facingX = cosf(angle);
		ptc->facingZ = sinf(angle);
		goalX = 0.0f;
		goalZ = 0.0f;
	}
}