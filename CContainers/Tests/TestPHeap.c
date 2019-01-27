#include "TestPHeap.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CContainers/PHeap.h>
#include <CContainers/PVector.h>
#include <CContainers/Utils.h>

#define TEST_HEAP_CAPACITY 10
PHeapHandle pVectorHeapHandle;
#define TEST_HEAP_NUMBERS_COUNT 20
int testNumbers [TEST_HEAP_NUMBERS_COUNT] = {
        10, 45, -13, 56, 12,
        45, 11, -1000, 27, 738,
        12, 11, 33, 57, -123,
        34, 56, 89, 191, 205
        };

static void PHeapSuite_Helper_PushTestNumbers ()
{
    for (uint index = 0; index  < TEST_HEAP_NUMBERS_COUNT; ++index)
    {
        int *new = malloc (sizeof (int));
        *new = testNumbers [index];
        PHeap_Push (pVectorHeapHandle, new);
    }
}

/// Integer min heap comparator.
static lint PHeapSuite_HeapComparator (const void *first, const void *second)
{
    return *((const int *) second) - *((const int *) first);
}

static void PHeapSuite_Setup ()
{
    pVectorHeapHandle = PHeap_Create (TEST_HEAP_CAPACITY, PHeapSuite_HeapComparator);
}

static void PHeapSuite_Teardown ()
{
    PHeap_Destruct (pVectorHeapHandle, PVector_Destruct, ContainerCallback_Free);
    CContainers_SetLastError (0);
}

static void PHeapSuite_PushAndTop ()
{
    PHeapSuite_Helper_PushTestNumbers ();
    int min = testNumbers [0];

    for (uint index = 1; index  < TEST_HEAP_NUMBERS_COUNT; ++index)
    {
        if (min > testNumbers [index])
        {
            min = testNumbers [index];
        }
    }

    int top = *((const int *) PHeap_Top (pVectorHeapHandle));
    if (min != top)
    {
        printf ("\n    Top item (e/a): %d, %d.", min, top);
        CU_FAIL ("Expected and actual values are not equal!");
    }
}

static void PHeapSuite_PushAndPopOrder ()
{
    PHeapSuite_Helper_PushTestNumbers ();
    int border = *((const int *) PHeap_Top (pVectorHeapHandle));
    PHeap_Pop (pVectorHeapHandle);

    while (PHeap_Size (pVectorHeapHandle) > 0)
    {
        int next = *((const int *) PHeap_Top (pVectorHeapHandle));
        PHeap_Pop (pVectorHeapHandle);

        if (next < border)
        {
            printf ("\n    Error: next value (%d) has higher priority than previous (%d)!",
                    next, border);
            CU_FAIL_FATAL ("Incorrect heap values order!");
        }

        border = next;
    }
}

static void PHeapSuite_Heapify ()
{
    PVectorHandle initialVector = PVector_Create (TEST_HEAP_NUMBERS_COUNT);
    for (uint index = 0; index < TEST_HEAP_NUMBERS_COUNT; ++index)
    {
        int *new = malloc (sizeof (int));
        *new = testNumbers [index];
        PVector_Insert (initialVector, PVector_End (initialVector), new);
    }

    PHeapHandle heap = PHeap_Heapify (initialVector, PVector_AsISizedContainer (),
            PVector_AsIIterableContainer (), PVectorIterator_AsIBiDirectionalIterator (),
            PVector_AsIMutableContainer (), PHeapSuite_HeapComparator, 0);

    for (uint index = 0; index < TEST_HEAP_NUMBERS_COUNT / 2; index++)
    {
        int value = **((int **) PVectorIterator_ValueAt (PVector_At (initialVector, index)));
        uint leftIndex = index * 2 + 1;
        uint rightIndex = index * 2 + 2;
        
        if (leftIndex < TEST_HEAP_NUMBERS_COUNT)
        {
            int leftValue = **((int **) PVectorIterator_ValueAt (PVector_At (initialVector, leftIndex)));
            if (leftValue < value)
            {
                printf ("\n    Error: %d is child of %d!", leftValue, value);
                CU_FAIL_FATAL ("Incorrect heap values order!");
            }
        }

        if (rightIndex < TEST_HEAP_NUMBERS_COUNT)
        {
            int rightValue = **((int **) PVectorIterator_ValueAt (PVector_At (initialVector, rightIndex)));
            if (rightValue < value)
            {
                printf ("\n    Error: %d is child of %d!", rightValue, value);
                CU_FAIL_FATAL ("Incorrect heap values order!");
            }
        }
    }
    
    PHeap_Destruct (heap, PVector_Destruct, ContainerCallback_Free);
}

void RegisterPHeapSuite ()
{
    CU_CI_DEFINE_SUITE ("PHeap", NULL, NULL, PHeapSuite_Setup, PHeapSuite_Teardown);
    CUNIT_CI_TEST (PHeapSuite_PushAndTop);
    CUNIT_CI_TEST (PHeapSuite_PushAndPopOrder);
    CUNIT_CI_TEST (PHeapSuite_Heapify);
}
