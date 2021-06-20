# CacheSim

`**This simulator does not support misaligned cache accesses**`

## Files
    1. Makefile : Builds the simulator
    2. helper_func.h : Printing stats
    3. structs.h : Structures used by the simulator
    4. cache_op.h : All the methods used for cache operation
    5. main.c : The main file

## Usage
    1. make
    2. ./csim [-verbosity] -s <set_bits> -E <associativity> -b <block_bits> -p <eviction_policy> -t <trace_file>
    3. make clean

## Eviction Policies Supported

    1) Least Recently Used (LRU)
    2) Pseudo Least Recently Used (PLRU)

## Trace Files
    1) trace/ contains all the trace files used to test the simulator
    2) trace files generated using valgrind(lackey)
   
## To Do
    1) Add more eviction policies
    2) Add support for misaligned cache accesses 
   
## Output
    The simulator outputs include:
    1. Number of reads
    2. Number of writes
    3. Number of cache hits
    4. Number of cache misses
    5. Number of cache evictions 
