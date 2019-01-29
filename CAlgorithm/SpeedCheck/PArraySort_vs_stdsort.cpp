#include "PArraySort_vs_stdsort.hpp"
#include <algorithm>
#include <ctime>
#include <vector>
#include <cstdio>

extern "C"
{
#include <CAlgorithm/PArraySort.h>
#include <CContainers/PVector.h>
#include <CContainers/Utils.h>
}

#define RAND_SEED 4543241
#define TEST_AMOUNT 3000000

static lint Comparator (const void *first, const void *second)
{
    return (const lint) second - (const lint) first;
}

static void **CreateVector ()
{
    void **vector = (void **) malloc (sizeof (lint) * TEST_AMOUNT);
    srand (RAND_SEED);

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        lint number = rand ();
        vector [index] = (void *) number;
    }

    return vector;
}

static clock_t Check_MergeSort ()
{
    void **vector = CreateVector ();
    clock_t begin = clock ();
    PArrayMergeSort (vector, vector + TEST_AMOUNT, Comparator);

    free (vector);
    return clock () - begin;
}

static clock_t Check_InplaceMergeSort ()
{
    void **vector = CreateVector ();
    clock_t begin = clock ();
    PArrayInplaceMergeSort (vector, vector + TEST_AMOUNT, Comparator);

    free (vector);
    return clock () - begin;
}

static clock_t Check_HeapSort ()
{
    void **vector = CreateVector ();
    clock_t begin = clock ();
    PArrayHeapSort (vector, vector + TEST_AMOUNT, Comparator);

    free (vector);
    return clock () - begin;
}

static std::vector <lint> *CreateStdVector ()
{
    auto *vector = new std::vector <lint> ();
    srand (RAND_SEED);

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        vector->push_back ((lint) rand ());
    }

    return vector;
}

static clock_t Check_stdsort ()
{
    auto *vector = CreateStdVector ();
    clock_t begin = clock ();
    std::sort (vector->begin (), vector->end ());

    delete vector;
    return clock () - begin;
}

static clock_t Check_stdsortheap ()
{
    auto *vector = CreateStdVector ();
    clock_t begin = clock ();

    std::make_heap (vector->begin (), vector->end ());
    std::sort_heap (vector->begin (), vector->end ());

    delete vector;
    return clock () - begin;
}

static clock_t Check_stdstablesort ()
{
    auto *vector = CreateStdVector ();
    clock_t begin = clock ();
    std::stable_sort (vector->begin (), vector->end ());

    delete vector;
    return clock () - begin;
}

void PArraySort_vs_stdsort ()
{
    printf ("PArraySort: amount of items -- %d.\n", TEST_AMOUNT);
    printf ("PArrayMergeSort: %dms.\n", (int) (Check_MergeSort () * 1000 / CLOCKS_PER_SEC));
    printf ("PArrayInplaceMergeSort: %dms.\n", (int) (Check_InplaceMergeSort () * 1000 / CLOCKS_PER_SEC));
    printf ("PArrayHeapSort: %dms.\n", (int) (Check_HeapSort () * 1000 / CLOCKS_PER_SEC));

    printf ("std::sort: %dms.\n", (int) (Check_stdsort () * 1000 / CLOCKS_PER_SEC));
    printf ("std::sort_heap: %dms.\n", (int) (Check_stdsortheap () * 1000 / CLOCKS_PER_SEC));
    printf ("std::stable_sort: %dms.\n", (int) (Check_stdstablesort () * 1000 / CLOCKS_PER_SEC));
}
