#pragma once
//#include "Animation.hpp"
#include "AnimationType.h"
#include "AnimationComponent.h"
#include <stdint.h>

#define BLEND_ANIM1_BRANCHLESS(comp) (comp->aAnimTime1 -= (float)(1 < comp->aAnimTime1) * int(comp->aAnimTime1))
#define BLEND_ANIM2_BRANCHLESS(comp) (comp->aAnimTime2 -= (float)(1 < comp->aAnimTime2) * int(comp->aAnimTime2))

struct BlendAnimationComponent
{
	//ANIMATION_TYPE aAnim1 = ANIMATION_IDLE;
	//uint8_t aAnimIdx1 = 0;
	//float aAnimTime1 = 0;
	//float aAnimTimeFactor1 = 1.0f;
	//
	//ANIMATION_TYPE aAnim2 = ANIMATION_IDLE;
	//uint8_t aAnimIdx2 = 0;
	//float aAnimTime2 = 0;
	//float aAnimTimeFactor2 = 1.0f;

	AnimationComponent anim1 = AnimationComponent();
	AnimationComponent anim2 = AnimationComponent();
};