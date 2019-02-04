#include "PODMemory_vs_memoryh.hpp"
#include <ctime>
#include <cstdio>
#include <cstdlib>

extern "C"
{
#include <memory.h>
#include <CContainers/PODMemory.h>
}

#define ALLOCATION_SIZE 10000000
#define ATTEMPTS 1000

static clock_t Check_PODMemory_Copy ()
{
    char *source = (char *) malloc (ALLOCATION_SIZE);
    char *output = (char *) malloc (ALLOCATION_SIZE);

    clock_t begin = clock ();
    for (int attempt = 0; attempt < ATTEMPTS; ++attempt)
    {
        PODMemory_Copy (output, source, ALLOCATION_SIZE);
    }

    return clock () - begin;
}

static clock_t Check_memcpy ()
{
    void *source = malloc (ALLOCATION_SIZE);
    void *output = malloc (ALLOCATION_SIZE);

    clock_t begin = clock ();
    for (int attempt = 0; attempt < ATTEMPTS; ++attempt)
    {
        memcpy (output, source, ALLOCATION_SIZE);
    }

    return clock () - begin;
}


void PODMemory_vs_memoryh ()
{
    printf ("PODMemory_Copy check, copying %d bytes.\n", ALLOCATION_SIZE);
    printf ("PODMemory_Copy: %dms.\n", (int) (Check_PODMemory_Copy () * 1000 / CLOCKS_PER_SEC));
    printf ("memcpy: %dms.\n", (int) (Check_memcpy () * 1000 / CLOCKS_PER_SEC));
}
