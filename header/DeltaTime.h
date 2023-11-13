#pragma once

void SetPaused(bool state);
void ResetRunTime();

const float& GetDeltaTime();
const float& GetRunTime();
void CountDeltaTime();
bool NewSecond();
float GetAverage();
int GetFPS();

extern float gameSpeed;