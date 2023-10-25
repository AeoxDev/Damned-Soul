#pragma once
//#include "Animation.hpp"
#include "AnimationType.h"
#include <stdint.h>
struct AnimationComponent
{
	ANIMATION_TYPE aAnim = ANIMATION_IDLE;
	uint8_t aAnimIdx = 0;
	float aAnimTime = 0;
	float aAnimTimeFactor = 1.0f;
};