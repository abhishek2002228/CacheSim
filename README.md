# CacheSim

## Files
    1. Makefile : Builds the simulator
    2. helper_func.h : Printing stats
    3. structs.h : Structures used by the simulator
    4. cache_op.h : All the methods used for cache operation
    5. main.c : The main file

## Usage
    1. make
    2. ./csim [-verbosity] -s <set_bits> -E <associativity> -b <block_bits> -p <char> -trace <file>
    3. make clean

## Eviction Policies Supported

    1) Least Recently Used (LRU)
    2) Pseudo Least Recently Used (PLRU)

## Trace Files
    1) trace/ contains all the trace files used to test the simulator
    2) trace files generated using valgrind(lackey)
   
## To Do
    1) Add more eviction policies
