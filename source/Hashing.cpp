#include "Hashing.h"

// djb2 | Do we need a faster hash?
uint64_t C_StringToHash(const char* c_str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *c_str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}