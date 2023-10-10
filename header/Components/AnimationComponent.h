#pragma once
#include "Animation.hpp"

struct AnimationComponent
{
	ANIMATION_TYPE aAnim = ANIMATION_IDLE;
	uint8_t aAnimIdx = 0;
	float aAnimTime = 0;
};