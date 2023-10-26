#pragma once

enum SquashStretchType
{
	Constant,
	Linear,
	Accelerating,
	Decelerating,
	SinusCurve//accelerating and deccelerating
};

struct SquashStretchComponent
{
	bool useStartScale = false;
	float startScaleX = 1.0f, startScaleY = 1.0f, startScaleZ = 1.0f;
	float goalScaleX = 1.0f, goalScaleY = 1.0f, goalScaleZ = 1.0f;
	SquashStretchType type;
};