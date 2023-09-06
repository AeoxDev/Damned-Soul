#pragma once

const float& GetDeltaTime();
void CountDeltaTime();
int DeltaTimeToMS();
bool NewSecond();
float GetAverage();
int GetFPS();

extern float gameSpeed;