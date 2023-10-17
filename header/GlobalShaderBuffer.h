#pragma once
#include <stdint.h>

struct GlobalShaderBuffer;

void CreateGlobalShaderBuffer();
void UpdateGlobalShaderBuffer();
int16_t GetGlobalShaderBufferIndex();

void GSBSetDirectionalLightColor(float r, float g, float b);
void GSBSetDirectionalLightDirection(float dirX, float dirY, float dirZ);
void GSBSetDirectionalLight(float r, float g, float b, float dirX, float dirY, float dirZ);