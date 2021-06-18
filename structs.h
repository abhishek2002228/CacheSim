#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>

typedef unsigned long long int address;

typedef struct
{
    int s;
    int b;
    long long E;
    char eviction_policy;

    long long hits;
    long long misses;
    long long evictions;

} Params;

typedef struct
{
    int valid;
    int tag;
    int LRU_cntr;
    //no block used because we don't care about the actual data present

} Line;

typedef struct
{
    Line **lines;

} Cache;

#endif