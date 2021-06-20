#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper_func.h"

void printStats(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
}

void printAccess(int reads, int writes)
{
    printf("reads:%d writes:%d\n", reads, writes);
}

void printUsage(char* argv[])
{
    printf("Usage: %s [-verbosity] -s <num> -E <num> -b <num> -p <char> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -p <char>  Eviction policy.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nEviction Policies: 'l' -> LRU, 'p' -> pseudo LRU");
    printf("\nExamples:\n");
    printf("  %s -s 4 -E 1 -b 4 -p l -t traces/yi.trace\n", argv[0]);
    printf("  %s -v -s 8 -E 2 -b 4 -p l -t traces/yi.trace\n", argv[0]);
    exit(0);
}