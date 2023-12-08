#pragma once
#include "MemLib/ML_String.hpp"

void SetPaused(bool state);
void ResetRunTime();
void SetVisualTimer(bool state);
bool GetVisualTimer();

void SetVisualFPS(bool state);
bool GetVisualFPS();

ML_String GetDigitalMinuteClock();

const float& GetDeltaTime();
const double& GetRunTime();
const int& GetSeconds();

float GetFrameTime();//Not affected by gamespeed
void CountDeltaTime();
bool NewSecond();
float GetAverage();
int GetFPS();

extern float gameSpeed;