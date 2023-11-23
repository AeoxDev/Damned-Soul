#include "AnimationComponent.h"
#include <cmath>

float AnimationComponent::GetTimeValue() const
{
	return std::powf(aAnimTime, aAnimTimePower);
}
