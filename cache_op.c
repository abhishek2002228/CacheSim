#include "cache_op.h"
#include <stdlib.h>
#include <stdbool.h>

Cache *buildCache(size_m E, size_c S, size_c B)
{
    Cache *cache = (Cache *)malloc(sizeof(Cache));
    cache->lines = (Line **)malloc(sizeof(Line *) * S);

    int setIndex;
    for (setIndex = 0; setIndex < S; setIndex++)
    {
        cache->lines[setIndex] = (Line *)malloc(sizeof(Line) * E);     
    }

    return cache;
}

void initCache(Cache *cache, size_m E, size_c S)
{
    int i, j;
    for (i = 0; i<S; i++)
    {
        for (j = 0; j < E; j++)
        {
            cache->lines[i][j].tag = 0;
            cache->lines[i][j].valid = 0;
            cache->lines[i][j].LRU_cntr = 0;
        }
    }
}

void freeCache(Cache *cache, long long S)
{
    int setIndex;
    for (setIndex = 0; setIndex < S; setIndex++)
    {
        free(cache->lines[setIndex]);
    }
    free(cache->lines);
    free(cache);
}

int LRU_miss(Cache *cache, int setIndex, Params *params)
{
    params->misses++;
    int i;
    int min = cache->lines[setIndex][0].LRU_cntr;
    int min_pos = 0;
    for (i = 0; i < params->E; i++)
    {
        if(cache->lines[setIndex][i].LRU_cntr < min)
        {
            min = cache->lines[setIndex][i].LRU_cntr;
            min_pos = i;
        }
    }
    for (i = 0; i < params->E; i++)
    {
        cache->lines[setIndex][i].LRU_cntr--;
    }
    cache->lines[setIndex][min_pos].LRU_cntr = params->E;

    if(cache->lines[setIndex][min_pos].valid)
    {
        params->evictions++;
    }

    return min_pos;
}

int p_LRU_miss(Cache *cache, int setIndex, Params *params)
{
    params->misses++;
    if(params->E == 1)
    {
        cache->lines[setIndex][0].LRU_cntr = 1;
        if(cache->lines[setIndex][0].valid == 1)
        {
            params->evictions++;
        }
        return 0;
    }
    int pos = -1;
    int cntr = 0;
    int i;
    for (i = 0; i < params->E; i++)
    {
        if(cache->lines[setIndex][i].LRU_cntr == 0)
        {
            pos = i;
            cntr++;
        }
    }
    if(cntr == 1)//all 1 LRU case incoming
    {
        for (i = 0; i < params->E; i++)
        {
            cache->lines[setIndex][i].LRU_cntr = 0;
        }
        cache->lines[setIndex][pos].LRU_cntr = 1;
        if(cache->lines[setIndex][pos].valid == 1)
        {
            params->evictions++;
        }
        return pos;
    }

    cache->lines[setIndex][pos].LRU_cntr = 1;
    if(cache->lines[setIndex][pos].valid == 1)
    {
        params->evictions++;
    }
    return pos;
}

void LRU_hit(Cache *cache, int setIndex, int lineIndex, Params *params)
{
    params->hits++;
    int i;
    for (i = 0; i < params->E; i++)
    {
        if(cache->lines[setIndex][i].LRU_cntr > cache->lines[setIndex][lineIndex].LRU_cntr)
        {
            cache->lines[setIndex][i].LRU_cntr--;
        }
    }
    cache->lines[setIndex][lineIndex].LRU_cntr = params->E;
}

void p_LRU_hit(Cache *cache, int setIndex, int lineIndex, Params *params)
{
    params->hits++;
    int i;
    int cntr = 0;
    for (i = 0; i < params->E; i++)
    {
        if(cache->lines[setIndex][i].LRU_cntr == 0)
        {
            cntr++;
        }
    }
    if(cntr == 1)
    {
        for (i = 0; i < params->E; i++)
        {
            cache->lines[setIndex][i].LRU_cntr = 0;
        }
        cache->lines[setIndex][lineIndex].LRU_cntr = 1;
    }
    else
    {
        cache->lines[setIndex][lineIndex].LRU_cntr = 1;
    }
    
}

hme runCache(Cache *cache, Params *params, address addr)
{
    int tagBits = 64 - (params->b + params->s);
    address tag = addr >> (params->b + params->s);
    address setIndex = (addr << tagBits) >> (tagBits + params->b);
    hme results;
    results.hm = 0;
    results.ev = 0;

    int i;
    for (i = 0; i < params->E; i++)
    {
        if((cache->lines[setIndex][i].tag == tag) && (cache->lines[setIndex][i].valid == 1))
        {
            switch (params->eviction_policy)
            {
            case 'l':
                LRU_hit(cache, setIndex, i, params);
                break;
            case 'p':
                p_LRU_hit(cache, setIndex, i, params);
                break;
            default:
                LRU_hit(cache, setIndex, i, params);
                break;
            }
            results.hm = 1;
            results.ev = 0;
            return results;
        }
    }
    //misses
    results.hm = 0;
    int ev_old = params->evictions;
    int miss_pos;
    switch (params->eviction_policy)
    {
    case 'l':
        miss_pos = LRU_miss(cache, setIndex, params);
        break;
    case 'p':
        miss_pos = p_LRU_miss(cache, setIndex, params);
        break;
    default:
        miss_pos = LRU_miss(cache, setIndex, params);
        break;
    }
    cache->lines[setIndex][miss_pos].tag = tag;
    cache->lines[setIndex][miss_pos].valid = 1;
    if(ev_old < params->evictions)
    {
        results.ev = 1;
    }
    else
    {
        results.ev = 0;
    }

    return results;
}