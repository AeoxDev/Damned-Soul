#include "DeltaTime.h"
#include <chrono>

#define DELTACAP 1.f / 30.f

float gameSpeed = 1.0f;
float secondTime = 0.0f;
float fps = 0.f;
float lastFPS = 0.0f;
float average;
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
	secondTime += deltaTime;
	deltaTime /= gameSpeed;
	fps+=1.0f;
}

int DeltaTimeToMS()
{
	return (int)(deltaTime*1000.0f);
}

bool NewSecond()
{
	if (secondTime >= 1)
	{
		average = 1.0f / fps;
		lastFPS = fps;
		fps = 0;
		secondTime -= 1.0f;
		return true;
	}
	return false;
}

float GetAverage()
{
	return average;
}

int GetFPS()
{
	return (int)lastFPS;
}


