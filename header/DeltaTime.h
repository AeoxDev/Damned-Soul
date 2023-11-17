#pragma once

void SetPaused(bool state);
void ResetRunTime();
void SetVisualTimer(bool state);
bool GetVisualTimer();

const float& GetDeltaTime();
const double& GetRunTime();
const int& GetSeconds();

float GetFrameTime();//Not affected by gamespeed
void CountDeltaTime();
bool NewSecond();
float GetAverage();
int GetFPS();

extern float gameSpeed;