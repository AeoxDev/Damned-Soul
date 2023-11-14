#pragma once

const float& GetDeltaTime();
float GetFrameTime();//Not affected by gamespeed
void CountDeltaTime();
bool NewSecond();
float GetAverage();
int GetFPS();

extern float gameSpeed;