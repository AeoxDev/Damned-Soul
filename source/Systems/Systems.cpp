#include "Systems\Systems.h"

void SetTime(System& systemName, float updateCallTime)
{
	systemName.timeCap = updateCallTime;
}
