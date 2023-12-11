#pragma once
//#include "Animation.hpp"
#include "AnimationType.h"
#include "AnimationComponent.h"
#include <stdint.h>

#define BLEND_ANIM1_BRANCHLESS(comp) (comp->aAnimTime1 -= (float)(1 < comp->aAnimTime1) * int(comp->aAnimTime1))
#define BLEND_ANIM2_BRANCHLESS(comp) (comp->aAnimTime2 -= (float)(1 < comp->aAnimTime2) * int(comp->aAnimTime2))

struct BlendAnimationComponent
{
	AnimationComponent lower = AnimationComponent(); //Lower body animations
	AnimationComponent upper = AnimationComponent(); //Upper body animations
};