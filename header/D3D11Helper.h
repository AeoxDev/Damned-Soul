#pragma once
#include <cinttypes>
//This contains helper functions for setting up renderable objects
// and manipulating their render states.

enum SHADER_TO_BIND_BUFFER
{
	BIND_VERTEX,
	BIND_HULL,
	BIND_DOMAIN,
	BIND_GEOMETRY,
	BIND_PIXEL
};

// Load a Pixel Shader by name (ps.cso) and return a global index that can be used to reference it
int8_t LoadPixelShader(const char* name);
// Set a new pixel shader by index
bool SetPixelShader(const int8_t idx);

// Load a Vertex Shader by name (vs.cso) and return a global index that can be used to reference it
// Also creates an input layout to accompany it
int8_t LoadVertexShader(const char* name);
// Set a new vertex (and accompanying input layout) shader by index
bool SetVertexShader(const int8_t idx);


// Create a constant buffer with provided data and return a unique index to it
int16_t CreateConstantBuffer(const void* data, const size_t size, const SHADER_TO_BIND_BUFFER& bindto, const uint8_t slot);
// Set an active constant buffer by index (shader and slot data contained in buffer)
bool SetConstantBuffer(const int16_t idx);
// Update a constant buffer by index with given data
bool UpdateConstantBuffer(const int16_t, const void* data, const size_t size);