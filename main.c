#include "cache_op.h"
#include "helper_func.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    Params *params = (Params *)malloc(sizeof(Params));
    char c;
    char *trace_file;
    int verbosity;
    while ((c = getopt(argc, argv, "s:E:b:t:p:vh")) != -1)
    {
        switch(c)
		{
        case 's':
            params->s = atoi(optarg);
            break;
        case 'E':
            params->E = atoi(optarg);
            break;
        case 'b':
            params->b = atoi(optarg);
            break;
        case 't':
            trace_file = optarg;
            break;
        case 'p':
            params->eviction_policy = *optarg;
            break;
        case 'v':
            verbosity = 1;
            break;
        case 'h':
            printUsage(argv);
            exit(0);
        default:
            printUsage(argv);
            exit(1);
        }
    }

    if (params->s == 0 || params->E == 0 || params->b == 0 || trace_file == NULL) 
	{
        printf("%s: Missing required command line argument\n", argv[0]);
        printUsage(argv);
        exit(1);
    }

    size_c S = pow(2, params->s);
    size_c B = pow(2, params->b);
    params->hits = 0;
    params->misses = 0;
    params->evictions = 0;

    printf("Simulating Cache....\n\n");
    Cache *cache = buildCache(params->E, S, B);
    initCache(cache, params->E, S);

    FILE *read_trace = fopen(trace_file, "r");
    assert(read_trace);
    char instruction;
    address addr;
    int size;
    hme read_results;
    int reads = 0;
    int writes = 0;
    if (read_trace)
    {
        while(fscanf(read_trace, " %c %llx,%d", &instruction, &addr, &size) == 3)
        {
            switch(instruction)
            {
				case 'I':
					break;
				case 'L':
                    read_results = runCache(cache, params, addr);
                    reads++;
                    if (verbosity)
                    {
                        if(read_results.hm)
                        {
                            printf("%c %llx,%d hit\n", instruction, addr, size);
                        }
                        else
                        {
                            printf("%c %llx,%d miss ", instruction, addr, size);
                            if(read_results.ev)
                            {
                                printf("eviction\n");
                            }
                            else
                            {
                                printf("\n");
                            }
                        }
                    }
                    break;
                case 'S':
                    read_results = runCache(cache, params, addr);
                    writes++;
                    if (verbosity)
                    {
                        if(read_results.hm)
                        {
                            printf("%c %llx,%d hit\n", instruction, addr, size);
                        }
                        else
                        {
                            printf("%c %llx,%d miss ", instruction, addr, size);
                            if(read_results.ev)
                            {
                                printf("eviction\n");
                            }
                            else
                            {
                                printf("\n");
                            }
                        }
                    }
					break;
				case 'M':
                    read_results = runCache(cache, params, addr);
                    reads++;
                    if(verbosity)
                    {
                        if (read_results.hm)
                        {
                            printf("%c %llx,%d hit ", instruction, addr, size);
                        }
                        else
                        {
                            printf("%c %llx,%d miss ", instruction, addr, size);
                            if(read_results.ev)
                            {
                                printf("eviction ");
                            }
                        }
                    }
                    read_results = runCache(cache, params, addr);
                    writes++;
                    if(verbosity)
                    {
                        if (read_results.hm)
                        {
                            printf("hit\n");
                        }
                        else
                        {
                            printf("miss ");
                            if(read_results.ev)
                            {
                                printf("eviction\n");
                            }
                            else
                            {
                                printf("\n");
                            }
                        }
                    }
					break;
				default:
                    printf("Incompatible trace provided\n");
                    break;
            }
        }
    }
    if(verbosity)
    {
    printf("\n\n");
    }
    printf("Printing results....\n");

    printf("\n");

    printAccess(reads, writes);
    printStats(params->hits, params->misses, params->evictions);
    freeCache(cache, S);
    free(params);
    fclose(read_trace);

    return 0;
}