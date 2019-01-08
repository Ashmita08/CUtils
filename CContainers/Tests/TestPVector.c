#include "TestPVector.h"
#include <stdlib.h>

#include <cunit/CUnit.h>
#include <cunit/CUnitCI.h>

#include <CContainers/PVector.h>
#include <CContainers/Utils.h>

#define INITIAL_CAPACITY 5
PVectorHandle pVectorHandle;

void PVectorSuite_Helper_CheckSize (uint size)
{
    if (size != PVector_Size (pVectorHandle))
    {
        printf ("\n        Size (e/a): %d, %d.", size, PVector_Size (pVectorHandle));
        CU_FAIL_FATAL ("Expected and equal values are not equal!");
    }
}

void PVectorSuite_Helper_IterateCheckNaturalOrdered ()
{
    int number = 0;
    PVectorIterator iterator = PVector_Begin (pVectorHandle);

    while (iterator != PVector_End (pVectorHandle))
    {
        if (**((int **) PVectorIterator_ValueAt (iterator)) != number)
        {
            printf ("\n        Value at %d (e/a): %d, %d.", number, number,
                    **((int **) PVectorIterator_ValueAt (iterator)));
            CU_FAIL ("Vector content is incorrect!");
        }

        iterator = PVectorIterator_Next (iterator);
        ++number;
    }
}

void PVectorSuite_Helper_ItemAtCheckNaturalOrdered (int size)
{
    for (uint index = 0; index < size; ++index)
    {
        PVectorIterator iterator = PVector_At (pVectorHandle, index);
        if (CContainers_GetLastError () != 0)
        {
            printf ("\n        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }

        int value = **((int **) PVectorIterator_ValueAt (iterator));
        if (value != index)
        {
            printf ("\n        Value at %d (e/a): %d, %d.", index, index, value);
            CU_FAIL ("Vector content is incorrect!");
        }
    }
}

void PVectorSuite_Setup ()
{
    pVectorHandle = PVector_Create (INITIAL_CAPACITY);
    CContainers_SetLastError (0);
}

void PVectorSuite_Teardown ()
{
    PVector_Destruct (pVectorHandle, ContainerCallback_Free);
}

void PVectorSuite_EmptyCreateFree ()
{

}

void PVectorSuite_EmptyBeginEndIterator ()
{
    if (PVector_Begin (pVectorHandle) != PVector_End (pVectorHandle))
    {
        CU_FAIL_FATAL ("Begin and end must be equal");
    }
}

void PVectorSuite_InsertBackAndIterateInitialCapacity ()
{
    for (int number = 0; number < INITIAL_CAPACITY; ++number)
    {
        int *value = malloc (sizeof (int));
        *value = number;
        PVector_Insert (pVectorHandle, PVector_End (pVectorHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PVectorSuite_Helper_CheckSize (INITIAL_CAPACITY);
    PVectorSuite_Helper_IterateCheckNaturalOrdered ();
    PVectorSuite_Helper_ItemAtCheckNaturalOrdered (INITIAL_CAPACITY);
}

void PVectorSuite_InsertBeginAndIterateInitialCapacity ()
{
    for (int number = INITIAL_CAPACITY - 1; number >= 0; --number)
    {
        int *value = malloc (sizeof (int));
        *value = number;
        PVector_Insert (pVectorHandle, PVector_Begin (pVectorHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PVectorSuite_Helper_CheckSize (INITIAL_CAPACITY);
    PVectorSuite_Helper_IterateCheckNaturalOrdered ();
    PVectorSuite_Helper_ItemAtCheckNaturalOrdered (INITIAL_CAPACITY);
}

void PVectorSuite_InsertMiddleAndIterateInitialCapacity ()
{
    for (int number = 0; number < INITIAL_CAPACITY; number += 2)
    {
        int *value = malloc (sizeof (int));
        *value = number;
        PVector_Insert (pVectorHandle, PVector_End (pVectorHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PVectorIterator iterator = PVector_Begin (pVectorHandle);
    PVectorIterator nextIterator = PVectorIterator_Next (iterator);

    while (iterator != PVector_End (pVectorHandle) && nextIterator != PVector_End (pVectorHandle))
    {
        int right = **((int **) PVectorIterator_ValueAt (nextIterator));
        int left = **((int **) PVectorIterator_ValueAt (iterator));

        if (right - left != 1)
        {
            int *value = malloc (sizeof (int));
            *value = left + 1;
            PVector_Insert (pVectorHandle, nextIterator, value);
        }

        iterator = PVectorIterator_Next (iterator);
        nextIterator = PVectorIterator_Next (iterator);
    }

    PVectorSuite_Helper_CheckSize (INITIAL_CAPACITY);
    PVectorSuite_Helper_IterateCheckNaturalOrdered ();
    PVectorSuite_Helper_ItemAtCheckNaturalOrdered (INITIAL_CAPACITY);
}

void RegisterPVectorSuite ()
{
    CU_CI_DEFINE_SUITE ("PVector", NULL, NULL, PVectorSuite_Setup, PVectorSuite_Teardown);
    CUNIT_CI_TEST (PVectorSuite_EmptyCreateFree);
    CUNIT_CI_TEST (PVectorSuite_EmptyBeginEndIterator);
    CUNIT_CI_TEST (PVectorSuite_InsertBackAndIterateInitialCapacity);
    CUNIT_CI_TEST (PVectorSuite_InsertBeginAndIterateInitialCapacity);
    CUNIT_CI_TEST (PVectorSuite_InsertMiddleAndIterateInitialCapacity);
}
