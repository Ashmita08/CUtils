#include "PVector_vs_stdvector.hpp"
#include <ctime>
#include <vector>
#include <cstdio>

extern "C"
{
#include <ShortTypes.h>
#include <CContainers/PVector.h>
#include <CContainers/Utils.h>
}

#define ITEMS_TO_PUSH_BACK_TEST 50000000

void PVector_EmptyCallback (void **item)
{

}

clock_t PVector_IntPushBackAndDestroy (uint count, bool reserve)
{
    clock_t begin = clock ();
    PVectorHandle vector = PVector_Create (reserve ? count : 1);
    while (count--)
    {
        PVector_Insert (vector, PVector_End (vector), nullptr);
    }

    PVector_Destruct (vector, PVector_EmptyCallback);
    return clock () - begin;
}

clock_t stdvector_IntPushBackAndDestroy (uint count, bool reserve)
{
    clock_t begin = clock ();

    std::vector <int> *vector = new std::vector <int> (reserve ? count : 1);
    while (count--)
    {
        vector->push_back (0);
    }

    delete vector;
    return clock () - begin;
}

void PVector_vs_stdvector ()
{
    printf ("PVector: items to push back test: %d.\n", ITEMS_TO_PUSH_BACK_TEST);
    printf ("PVector int (no reserve): %dms.\n",
            (int) (PVector_IntPushBackAndDestroy (ITEMS_TO_PUSH_BACK_TEST, false) * 1000 / CLOCKS_PER_SEC));
    printf ("stdvector int (no reserve): %dms.\n",
            (int) (stdvector_IntPushBackAndDestroy (ITEMS_TO_PUSH_BACK_TEST, false) * 1000 / CLOCKS_PER_SEC));

    printf ("PVector int (reserve): %dms.\n",
            (int) (PVector_IntPushBackAndDestroy (ITEMS_TO_PUSH_BACK_TEST, true) * 1000 / CLOCKS_PER_SEC));
    printf ("stdvector int (reserve): %dms.\n",
            (int) (stdvector_IntPushBackAndDestroy (ITEMS_TO_PUSH_BACK_TEST, true) * 1000 / CLOCKS_PER_SEC));
}
