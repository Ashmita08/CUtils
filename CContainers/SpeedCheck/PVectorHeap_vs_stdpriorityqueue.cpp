#include "PVectorHeap_vs_stdpriorityqueue.hpp"
#include <ctime>
#include <queue>
#include <cstdio>
#include <cstdlib>

extern "C"
{
#include <ShortTypes.h>
#include <CContainers/PVectorHeap.h>
#include <CContainers/PVector.h>
#include <CContainers/Utils.h>
}

#define SRAND_SEED 154632
#define TEST_AMOUNT 10000000
#define DEFAULT_INITIAL_CAPACITY 100

static void EmptyDestruct (void **item)
{

}

static lint Comparator (const void *first, const void *second)
{
    return (lint) first - (lint) second;
}

clock_t PVectorHeap_Insert (bool reserve)
{
    clock_t begin = clock ();
    srand (SRAND_SEED);

    PVectorHeapHandle heap = PVectorHeap_Create (reserve ? TEST_AMOUNT : DEFAULT_INITIAL_CAPACITY, Comparator);
    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        PVectorHeap_Push (heap, (void *) rand ());
    }

    PVectorHeap_Destruct (heap, PVector_Destruct, EmptyDestruct);
    return clock () - begin;
}

clock_t stdpriorityqueue_Insert ()
{
    clock_t begin = clock ();
    srand (SRAND_SEED);
    auto queue = new std::priority_queue <int> ();

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        queue->push (rand ());
    }

    delete queue;
    return clock () - begin;
}

void PVectorHeap_vs_stdpriorityqueue ()
{
    printf ("PVectorHeap: items to insert -- %d.\n", TEST_AMOUNT);
    printf ("PVectorHeap, no reserve: %dms.\n", (int) (PVectorHeap_Insert (false) * 1000 / CLOCKS_PER_SEC));
    printf ("PVectorHeap, reserve: %dms.\n", (int) (PVectorHeap_Insert (true) * 1000 / CLOCKS_PER_SEC));
    printf ("std::priority_queue: %dms.\n", (int) (stdpriorityqueue_Insert () * 1000 / CLOCKS_PER_SEC));
}
