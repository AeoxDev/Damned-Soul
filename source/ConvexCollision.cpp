#include "ConvexCollision.h"

struct ConvexHull
{
	float centerX, centerY;
	float cornersX[16], cornersY[16];
	int amountOfCorners;
};

void CreateConvexHull(const float* cornersX,const float* cornersY, int& amountOfCorners)
{
	//Setup lines
	ConvexHull convexHull;//Create component of this instead

	//Calculate center

	//Return component
}
