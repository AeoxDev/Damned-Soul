#include "D3D11Helper.h"

uint64_t C_StringToHash(const char* c_str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *c_str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

TX_IDX LoadTexture(const char* name)
{
    uint64_t hash = C_StringToHash(name);
	return -1;
}