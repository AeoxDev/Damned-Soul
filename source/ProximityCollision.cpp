#include "Physics\Backend\ProximityCollision.h"
#include "Physics\Backend\Collision.h"

int GetOrientation(float& Ax, float& Az, float& Bx, float& Bz, float& Cx, float& Cz)
{
	float val = (Bz - Az) * (Cx - Bx) - (Bx - Ax) * (Cz - Bz);

	if (val == 0) return 0; //Collinear
	return (val > 0) ? 1 : 2; //Clockwise or counterclockwise
}

void ProximityCorrection(Registry& registry, EntityID& wall, int& index, float& x, float& z)
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
		if (((x - A.x) * (B.z - A.z) - (z - A.z) * (B.x - A.x)) >= 0)
		{
			first = true; //Right side or on the line
		}
		else
		{
			first = false; //Left side
		}

		if (((x - B.x) * (C.z - B.z) - (z - B.z) * (C.x - B.x)) >= 0)
		{
			second = true; //Right side or on the line
		}
		else
		{
			second = false; //Left side
		}

		if (first && second)
		{
			return; //Do nothing
		}
		else if (!first && !second)
		{
			ProximityStepChart(A, B, C, x, z, x, z); //Change second pair of x, z -> Previous position X and Z.
		}
		else
		{
			//Check if C -> (x,z) intersects with A -> B. If it does, then the "second" result is wrong and "first" is correct. If it does not intersect then it's vice versa.
			int o1 = GetOrientation(A.x, A.z, B.x, B.z, C.x, C.z);
			int o2 = GetOrientation(A.x, A.z, B.x, B.z, x, z);
			int o3 = GetOrientation(C.x, C.z, x, z, A.x, A.z);
			int o4 = GetOrientation(C.x, C.z, x, z, B.x, B.z);

			if ((o1 != o2) && (o3 != o4)) //Intersection did not occur
			{
				if (!first)
				{
					//Move using A -> B
					ProximityMove(A, B, x, z);
				}
			}
			else
			{
				if (!second)
				{
					//Move using B -> C
					ProximityMove(B, C, x, z);
				}
			}
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
		if (((x - A.x) * (B.z - A.z) - (z - A.z) * (B.x - A.x)) <= 0)
		{
			first = true; //Left side or on the line
		}
		else
		{
			first = false; //Right side
		}

		if (((x - B.x) * (C.z - B.z) - (z - B.z) * (C.x - B.x)) <= 0)
		{
			second = true; //Left side or on the line
		}
		else
		{
			second = false; //Right side
		}

		if (first && second)
		{
			return; //Do nothing
		}
		else if (!first && !second)
		{
			ProximityStepChart(A, B, C, x, z, x, z); //Change second pair of x, z -> Previous position X and Z.
		}
		else
		{
			//Check if C -> (x,z) intersects with A -> B. If it does, then the "second" result is wrong and "first" is correct. If it does not intersect then it's vice versa.
			int o1 = GetOrientation(A.x, A.z, B.x, B.z, C.x, C.z);
			int o2 = GetOrientation(A.x, A.z, B.x, B.z, x, z);
			int o3 = GetOrientation(C.x, C.z, x, z, A.x, A.z);
			int o4 = GetOrientation(C.x, C.z, x, z, B.x, B.z);

			if ((o1 != o2) && (o3 != o4)) //Intersection did not occur
			{
				if (!first)
				{
					//Move using A -> B
					ProximityMove(A, B, x, z);
				}
			}
			else
			{
				if (!second)
				{
					//Move using B -> C
					ProximityMove(B, C, x, z);
				}
			}
		}
	}
}

void ProximityMove(ProximityPoint& p1, ProximityPoint& p2, float& x, float& z)
{
	//d = (x - x1)(y2 - y1) - (y - y1)(x2 - x1)
	float magnitude = ((x - p1.x) * (p2.z - p1.z)) - ((z - p1.z) * (p2.x - p1.x));

	float dx = p2.x - p1.x;
	float dz = p2.z - p1.z;

	//If magnitude > 0 --> Normal = (-dz, dx)
	//If magnitude < 0 --> Normal = (dz, -dx)

	if (magnitude > 0)
	{
		//Inverse the delta z value.
		dz = -dz;

		//Calculate the coefficient that tells the amount to move
		if (((dx * p1.x) - (dz * p2.z) + (dz * p1.z) - (dx * p2.x)) == 0) //Division by zero is BAD! >:(
		{
			return;
		}
		float X = abs(((p1.x * p2.z) - (p1.x * z) - (x * p2.z) + (x * p1.z) + (z * p2.x) - (p1.z * p2.x)) / ((dx * p1.x) - (dz * p2.z) + (dz * p1.z) - (dx * p2.x)));

		//Move the position to be on the line.
		x = x + dz * X;
		z = z + dx * X;
	}
	else if (magnitude < 0)
	{
		//Inverse the delta x value.
		dx = -dx;

		//Calculate the coefficient that tells the amount to move
		if (((dx * p1.x) - (dz * p2.z) + (dz * p1.z) - (dx * p2.x)) == 0) //Division by zero is BAD! >:(
		{
			return;
		}
		float X = abs(((p1.x * p2.z) - (p1.x * z) - (x * p2.z) + (x * p1.z) + (z * p2.x) - (p1.z * p2.x)) / ((dx * p1.x) - (dz * p2.z) + (dz * p1.z) - (dx * p2.x)));

		//Move the position to be on the line.
		x = x + dz * X;
		z = z + dx * X;
	}
}

void ProximityStepChart(ProximityPoint& A, ProximityPoint& B, ProximityPoint& C, float& x, float& z, float& px, float& pz)
{
	int o1, o2, o3, o4;
	//Step 1: Check if A -> Previous position intersects with B -> C
	bool step1;
	o1 = GetOrientation(B.x, B.z, C.x, C.z, A.x, A.z);
	o2 = GetOrientation(B.x, B.z, C.x, C.z, px, pz);
	o3 = GetOrientation(A.x, A.z, px, pz, B.x, B.z);
	o4 = GetOrientation(A.x, A.z, px, pz, C.x, C.z);

	if ((o1 != o2) && (o3 != o4))
	{
		step1 = false; //No intersection
	}
	else
	{
		step1 = true; //Intersection
	}

	//Step 2: Check if C -> Previous position intersects with A -> B
	bool step2;
	o1 = GetOrientation(A.x, A.z, B.x, B.z, C.x, C.z);
	o2 = GetOrientation(A.x, A.z, B.x, B.z, px, pz);
	o3 = GetOrientation(C.x, C.z, px, pz, A.x, A.z);
	o4 = GetOrientation(C.x, C.z, px, pz, B.x, B.z);

	if ((o1 != o2) && (o3 != o4))
	{
		step2 = false; //No intersection
	}
	else
	{
		step2 = true; //Intersection
	}

	//Step 3: Split into cases based on which line intersected
	if (step1 && step2)
	{
		//Step 4a: Move entity to position of B
		x = B.x;
		z = B.z;
	}
	else if (!step1 && !step2)
	{
		//Step 4b: Move entity first based on magnitude from the first line, then move it again based on the magnitude of the second line.
		ProximityMove(A, B, x, z); //x and z gets updated
		ProximityMove(B, C, x, z); //x and z gets updated
	}
	else
	{
		//Step 4c: Only move based on which line did not intersect in Step 1 and 2.
		if (step1)
		{
			//Move based on the B -> C line
			ProximityMove(B, C, x, z);
		}
		else
		{
			//Move based on the A -> B line
			ProximityMove(A, B, x, z);
		}
	}

}
