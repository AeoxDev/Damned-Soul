#include "Physics\Backend\ProximityCollision.h"
#include "Physics\Backend\Collision.h"
#include "Registry.h"
#include "DeltaTime.h"
//include <algorithm>
#define maxVar(a,b)            (((a) > (b)) ? (a) : (b))
#define minVar(a,b)            (((a) < (b)) ? (a) : (b))

void FindIntersection(float& x, float& z, ProximityPoint& p1, ProximityPoint& p2, float& otherX, float& otherZ)
{
	//Calculates where two lines intersect, should only be used when division = 0 for the other cases.
	float a1 = p1.z - p2.z;
	float b1 = p2.x - p1.x;
	float c1 = a1 * p2.x + b1 * p2.z;

	float a2 = otherZ - z;
	float b2 = x - otherX;
	float c2 = a2 * x + b2 * z;

	float determinant = a1 * b2 - a2 * b1;

	x = (b2 * c1 - b1 * c2) / determinant;
	z = (a1 * c2 - a2 * c1) / determinant;
}

bool IntersectionOnLine(float& line1x1, float line1x2, float& line1z1, float line1z2, float& line2x1, float& line2x2, float& line2z1, float& line2z2)
{
	//Calculates where an intersection occured
	float a1 = line1z1 - line1z2;
	float b1 = line1x2 - line1x1;
	float c1 = a1 * line1x2 + b1 * line1z2;

	float a2 = line2z2 - line2z1;
	float b2 = line2x1 - line2x2;
	float c2 = a2 * line2x1 + b2 * line2z1;

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		return false;
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / determinant;
		if (x < line1x1 && x < line1x2)
		{
			x += 0.001f;
		}
		else if (x > line1x1 && x > line1x2)
		{
			x -= 0.001f;
		}

		float z = (a1 * c2 - a2 * c1) / determinant;
		if (z < line1z1 && z < line1z2)
		{
			z += 0.001f;
		}
		else if (z > line1z1 && z > line1z2)
		{
			z -= 0.001f;
		}

		if (x <= maxVar(line1x1, line1x2) && x >= minVar(line1x1, line1x2) && z <= maxVar(line1z1, line1z2) && z >= minVar(line1z1, line1z2))
		{
			return true;
		}
	}
	return false;
}

void ProximityCorrection(EntityID& wall, int& index, float& x, float& z, float& previousX, float& previousZ, float& speedX, float& speedZ)
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
			ProximityStepChart(A, B, C, x, z, previousX, previousZ); //Go through the step chart.
		}
		else
		{
			//Check if C -> (x,z) intersects with A -> B. If it does, then the "second" result is wrong and "first" is correct. If it does not intersect then it's vice versa.
			bool intersect = IntersectionOnLine(C.x, x, C.z, z, A.x, B.x, A.z, B.z);

			if (intersect)
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

			if ((abs(x) >= abs(B.x) - 0.001f) && (abs(x) <= abs(B.x) + 0.001f)) //Please don't place the entity on the line cause it will mess things up in 90 degree corners.
			{
				if (!first)
				{
					//Check B->C line X.
					if ((previousX < B.x) && (previousX < C.x))
					{
						x -= 0.01f;
					}
					else
					{
						x += 0.01f;
					}
				}
				if (!second)
				{
					//Check B->A line X.
					if ((previousX < B.x) && (previousX < A.x))
					{
						x -= 0.01f;
					}
					else
					{
						x += 0.01f;
					}
				}
			}
			
			if((abs(z) >= abs(B.z) - 0.001f) && (abs(z) <= abs(B.z) + 0.001f)) //Please don't place the entity on the line cause it will mess things up in 90 degree corners.
			{
				if (!first)
				{
					//Check B->C line Z.
					if ((previousZ < B.z) && (previousZ < C.z))
					{
						z -= 0.01f;
					}
					else
					{
						z += 0.01f;
					}
				}
				if (!second)
				{
					//Check B->A line Z.
					if ((previousZ < B.z) && (previousZ < A.z))
					{
						z -= 0.01f;
					}
					else
					{
						z += 0.01f;
					}
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
			//First point = (x, z), Second point = ((-dz * magnitude), (-dx * magnitude)), Third point = (p1.x, p1.z), Fourth point = (p2.x, p2.z)
			float otherX = x - dz * magnitude;
			float otherZ = z - dx * magnitude;
			FindIntersection(x, z, p1, p2, otherX, otherZ);
			return;
		}
		float X = abs(((p1.x * p2.z) - (p1.x * z) - (x * p2.z) + (x * p1.z) + (z * p2.x) - (p1.z * p2.x)) / ((dx * p1.x) - (dz * p2.z) + (dz * p1.z) - (dx * p2.x)));

		//Move the position to be on the line.
		x = x + dz * X * 1.1f;
		z = z + dx * X * 1.1f;
	}
	else if (magnitude < 0)
	{
		//Inverse the delta x value.
		dx = -dx;

		//Calculate the coefficient that tells the amount to move
		if (((dx * p1.x) - (dz * p2.z) + (dz * p1.z) - (dx * p2.x)) == 0) //Division by zero is BAD! >:(
		{
			//First point = (x, z), Second point = ((-dz * magnitude), (-dx * magnitude)), Third point = (p1.x, p1.z), Fourth point = (p2.x, p2.z)
			float otherX = x - dz * magnitude;
			float otherZ = z - dx * magnitude;
			FindIntersection(x, z, p1, p2, otherX, otherZ);
			return;
		}
		float X = abs(((p1.x * p2.z) - (p1.x * z) - (x * p2.z) + (x * p1.z) + (z * p2.x) - (p1.z * p2.x)) / ((dx * p1.x) - (dz * p2.z) + (dz * p1.z) - (dx * p2.x)));

		//Move the position to be on the line.
		x = x + dz * X * 1.1f;
		z = z + dx * X * 1.1f;
	}
}

void ProximityStepChart(ProximityPoint& A, ProximityPoint& B, ProximityPoint& C, float& x, float& z, float& px, float& pz)
{
	//Step 1: Check if A -> Previous position intersects with B -> C
	bool step1 = IntersectionOnLine(A.x, px, A.z, pz, B.x, C.x, B.z, C.z);

	//Step 2: Check if C -> Previous position intersects with A -> B
	bool step2 = IntersectionOnLine(C.x, px, C.z, pz, A.x, B.x, A.z, B.z);

	//Step 3: Split into cases based on which line intersected
	if (step1 && step2)
	{
		//Step 4a: Handle the corner case.
		ProximityCornerMove(A, B, C, x, z);
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

void ProximityCornerMove(ProximityPoint& A, ProximityPoint& B, ProximityPoint& C, float& x, float& z)
{
	//Find the two normals. Move along both the normals.
	float difABX = B.x - A.x;
	float difABZ = B.z - A.z;
	
	float difBCX = C.x - B.x;
	float difBCZ = C.z - B.z;

	float normalABX = -difABZ;
	float normalABZ = difABX;

	float normalBCX = -difBCZ;
	float normalBCZ = difBCX;

	//Normalize the normals
	float len = sqrtf((normalABX * normalABX) + (normalABZ * normalABZ));
	normalABX /= len;
	normalABZ /= len;
	len = sqrtf((normalBCX * normalBCX) + (normalBCZ * normalBCZ));
	normalBCX /= len;
	normalBCZ /= len;

	x = B.x - 0.1f * (normalABX + normalBCX);
	z = B.z - 0.1f * (normalABZ + normalBCZ);
}
