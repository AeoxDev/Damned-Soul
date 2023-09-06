#include "DeltaTime.h"
#include <chrono>

#define DELTACAP 1.f / 30.f


float gameSpeed = 1.0f;
float deltaTime;
const float& GetDeltaTime()
{
	return deltaTime;
}

void CountDeltaTime()
{
	static std::chrono::steady_clock::time_point time;
	static std::chrono::steady_clock::time_point end;
	static std::chrono::duration<float> deltaTimeCount;
	end = std::chrono::steady_clock::now();
	deltaTimeCount = end - time;

	time = std::chrono::steady_clock::now();
	deltaTime = deltaTimeCount.count() ;
	if (deltaTimeCount.count() > DELTACAP)
	{
		deltaTime = DELTACAP;
	}
	deltaTime /= gameSpeed;
}
