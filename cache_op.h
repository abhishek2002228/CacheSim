#ifndef CACHE_OP_H
#define CACHE_OP_H

#include "helper_func.h"
#include "structs.h"

typedef unsigned long long int size_c;
typedef long long int size_m;

typedef struct
{
    int hm; // 1 if hit , 0 if miss
    int ev; // 1 if eviction, 0 if no eviction

} hme;

Cache *buildCache(size_m E, size_c S, size_c B);

void initCache(Cache *cache, size_m E, size_c S);

void freeCache(Cache *cache, long long S);

hme runCache(Cache *cache, Params *params, address addr);

int LRU_miss(Cache *cache, int setIndex, Params *params);

void LRU_hit(Cache *cache, int setIndex, int lineIndex, Params *params);

int p_LRU_miss(Cache *cache, int setIndex, Params *params);

void p_LRU_hit(Cache *cache, int setIndex, int lineIndex, Params *params);

#endif