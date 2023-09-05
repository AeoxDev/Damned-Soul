#pragma once
#include <cinttypes>
//This contains helper functions for setting up renderable objects
// and manipulating their render states.

// Load a Pixel Shader by name (ps.cso) and return a global index that can be used to reference it
int8_t LoadPixelShader(const char* name);
// Set a new pixel shader by index
bool SetPixelShader(int8_t idx);

// Load a Vertex Shader by name (vs.cso) and return a global index that can be used to reference it
// Also creates an input layout to accompany it
int8_t LoadVertexShader(const char* name);
// Set a new vertex (and accompanying input layout) shader by index
bool SetVertexShader(int8_t idx);