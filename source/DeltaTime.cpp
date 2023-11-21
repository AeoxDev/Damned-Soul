#include "DeltaTime.h"
#include "UIRenderer.h"
#include "Relics/RelicFunctions.h"
#include "Camera.h"
#include "States\StateManager.h"
#include <chrono>

#define DELTACAP 1.f / 30.f

float gameSpeed = 1.0f;
float secondTime = 0.0f;
float fps = 0.f;
float lastFPS = 0.0f;
float average;
float deltaTime;
float frameTime;

double runTime = 0.0;
int seconds = 0;
bool paused = true;
bool showTimer = false;

void SetPaused(bool state)
{
	paused = state;
}

void ResetRunTime()
{
	runTime = 0.0;
	seconds = 0;
}

void SetVisualTimer(bool state)
{
	showTimer = state;
}

bool GetVisualTimer()
{
	return showTimer;
}

ML_String GetDigitalMinuteClock()
{
	DSINT2 time = {};

	time.x = GetSeconds() / 60;
	time.y = GetSeconds() % 60;

	ML_String minutes, seconds, totalTime;

	if (time.x < 10)
	{
		minutes = "0";
		minutes.append(std::to_string(time.x).c_str());
	}
	else
		minutes = std::to_string(time.x).c_str();

	if (time.y < 10)
	{
		seconds = "0";
		seconds.append(std::to_string(time.y).c_str());
	}
	else
		seconds = std::to_string(time.y).c_str();

	totalTime = "Time: ";
	totalTime.append(minutes);
	totalTime.append(":");
	totalTime.append(seconds);

	return totalTime;
}

const float& GetDeltaTime()
{
	return deltaTime;
}

const double& GetRunTime()
{
	return runTime;
}

const int& GetSeconds()
{
	return seconds;
}

float GetFrameTime()
{
	return frameTime;
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
	frameTime = deltaTime;
	
	deltaTime *= gameSpeed;
	fps+=1.0f;


	// For now only, should be moved to a better place later
	auto timeUpdateRelics = Relics::GetFunctionsOfType(Relics::FUNC_ON_FRAME_UPDATE);
	for (uint32_t i = 0; i < timeUpdateRelics.size(); ++i)
	{
		timeUpdateRelics[i](&deltaTime);
	}
}

bool NewSecond()
{
	if (secondTime >= 1)
	{
		average = 1.0f / fps;
		lastFPS = fps;
		fps = 0;
		secondTime -= 1.0f;

		runTime += 1.0;

		if (!paused && !Camera::InCutscene() && !(currentStates & InMainMenu))
		{
			seconds += 1;
			RedrawUI();
		}
		
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


