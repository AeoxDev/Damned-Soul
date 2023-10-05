#pragma once
#include <cinttypes>

// djb2 | Do we need a faster hash?
uint64_t C_StringToHash(const char* c_str);