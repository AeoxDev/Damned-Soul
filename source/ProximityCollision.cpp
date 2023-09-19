#include "Physics\Backend\ProximityCollision.h"
#include "Physics\Backend\Collision.h"

int GetOrientation(float Ax, float Az, float Bx, float Bz, float Cx, float Cz)
{
	float val = (Bz - Az) * (Cx - Bx) - (Bx - Ax) * (Cz - Bz);

	if (val == 0) return 0; //Collinear
	return (val > 0) ? 1 : 2; //Clockwise or counterclockwise
}

bool IsProximityCorrectSide(Registry& registry, EntityID& wall, int index, float x, float z)
{
	ProximityHitboxComponent* wallHitbox = registry.GetComponent<ProximityHitboxComponent>(wall);

	ProximityPoint A, B, C;

	if (wallHitbox->clockwise)
	{
		B = wallHitbox->pointList[index];
		if (index == 0) //First point is at list.size() - 1, last at 1.
		{
			A = wallHitbox->pointList[(int)(wallHitbox->pointList.size() - 1)];
			C = wallHitbox->pointList[1];
		}
		else if (index == (wallHitbox->pointList.size() - 1)) //First point is at index - 1, last at 0;
		{
			A = wallHitbox->pointList[index - 1];
			C = wallHitbox->pointList[0];
		}
		else //First point is at index - 1, last is at index + 1.
		{
			A = wallHitbox->pointList[index - 1];
			C = wallHitbox->pointList[index + 1];
		}

		//Check if (x,z) is to the right of the two lines
		bool first, second;
		if (((x - A.x) * (B.z - A.z) - (z - A.z) * (B.z - A.z)) >= 0)
		{
			first = true; //Right side or on the line
		}
		else
		{
			first = false; //Left side
		}

		if (((x - B.x) * (C.z - B.z) - (z - B.z) * (C.z - B.z)) >= 0)
		{
			second = true; //Right side or on the line
		}
		else
		{
			second = false; //Left side
		}

		if (first && second)
		{
			return true;
		}
		else if (!first && !second)
		{
			return false;
		}
		else
		{
			//Check if C -> (x,z) intersects with A -> B. If it does, then the "second" result is wrong and "first" is correct. If it does not intersect then it's vice versa.
			int o1 = GetOrientation(A.x, A.z, B.x, B.z, C.x, C.z);
			int o2 = GetOrientation(A.x, A.z, B.x, B.z, x, z);
			int o3 = GetOrientation(C.x, C.z, x, z, A.x, A.z);
			int o4 = GetOrientation(C.x, C.z, x, z, B.x, B.z);

			if ((o1 != o2) && (o3 != o4))
			{
				return first; //Intersection occurred
			}
			return second;
		}


	}
	else
	{
		B = wallHitbox->pointList[index];
		if (index == 0) //First point is at 1, last at list.size() - 1.
		{
			A = wallHitbox->pointList[1];
			C = wallHitbox->pointList[(int)(wallHitbox->pointList.size() - 1)];
		}
		else if (index == (wallHitbox->pointList.size() - 1)) //First point is at 0, last is at index - 1;
		{
			A = wallHitbox->pointList[0];
			C = wallHitbox->pointList[index - 1];
		}
		else //First point is at index + 1, last is at index - 1.
		{
			A = wallHitbox->pointList[index + 1];
			C = wallHitbox->pointList[index - 1];
		}

		//Check if (x,z) is to the left of the two lines
		bool first, second;
		if (((x - A.x) * (B.z - A.z) - (z - A.z) * (B.z - A.z)) <= 0)
		{
			first = true; //Left side or on the line
		}
		else
		{
			first = false; //Right side
		}

		if (((x - B.x) * (C.z - B.z) - (z - B.z) * (C.z - B.z)) <= 0)
		{
			second = true; //Left side or on the line
		}
		else
		{
			second = false; //Right side
		}

		if (first && second)
		{
			return true;
		}
		else if (!first && !second)
		{
			return false;
		}
		else
		{
			//Check if C -> (x,z) intersects with A -> B. If it does, then the "second" result is wrong and "first" is correct. If it does not intersect then it's vice versa.
			int o1 = GetOrientation(A.x, A.z, B.x, B.z, C.x, C.z);
			int o2 = GetOrientation(A.x, A.z, B.x, B.z, x, z);
			int o3 = GetOrientation(C.x, C.z, x, z, A.x, A.z);
			int o4 = GetOrientation(C.x, C.z, x, z, B.x, B.z);

			if ((o1 != o2) && (o3 != o4))
			{
				return first; //Intersection occurred
			}
			return second;
		}
	}
    return false;
}
