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
        int value = **((int **) PVectorIterator_ValueAt (iterator));
        if (value != number)
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

void PVectorSuite_Helper_InsertBackAndIterate (uint size)
{
    for (int number = 0; number < size; ++number)
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

    PVectorSuite_Helper_CheckSize (size);
    PVectorSuite_Helper_IterateCheckNaturalOrdered ();
    PVectorSuite_Helper_ItemAtCheckNaturalOrdered (size);
}

void PVectorSuite_Helper_InsertBeginAndIterate (uint size)
{
    for (int number = size - 1; number >= 0; --number)
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

    PVectorSuite_Helper_CheckSize (size);
    PVectorSuite_Helper_IterateCheckNaturalOrdered ();
    PVectorSuite_Helper_ItemAtCheckNaturalOrdered (size);
}

void PVectorSuite_Helper_InsertMiddleAndIterate (uint size)
{
    if (size % 2 == 0)
    {
        ++size;
    }

    for (int number = 0; number < size; number += 2)
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
            iterator = PVector_Insert (pVectorHandle, nextIterator, value);
        }
        else
        {
            iterator = PVectorIterator_Next (iterator);
        }

        nextIterator = PVectorIterator_Next (iterator);
    }

    PVectorSuite_Helper_CheckSize (size);
    PVectorSuite_Helper_IterateCheckNaturalOrdered ();
    PVectorSuite_Helper_ItemAtCheckNaturalOrdered (size);
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
    PVectorSuite_Helper_InsertBackAndIterate (INITIAL_CAPACITY);
}

void PVectorSuite_InsertBeginAndIterateInitialCapacity ()
{
    PVectorSuite_Helper_InsertBeginAndIterate (INITIAL_CAPACITY);
}

void PVectorSuite_InsertMiddleAndIterateInitialCapacity ()
{
    PVectorSuite_Helper_InsertMiddleAndIterate (INITIAL_CAPACITY);
}

void PVectorSuite_InsertBackAndIterateExtendedCapacity ()
{
    PVectorSuite_Helper_InsertBackAndIterate (INITIAL_CAPACITY * 2);
}

void PVectorSuite_InsertBeginAndIterateExtendedCapacity ()
{
    PVectorSuite_Helper_InsertBeginAndIterate (INITIAL_CAPACITY * 2);
}

void PVectorSuite_InsertMiddleAndIterateExtendedCapacity ()
{
    PVectorSuite_Helper_InsertMiddleAndIterate (INITIAL_CAPACITY * 2);
}

void PVectorSuite_EraseBegin ()
{
    uint size = 10;
    for (int number = 0; number < size; ++number)
    {
        int *value = malloc (sizeof (int));
        *value = number == 0 ? 0 : number - 1;
        PVector_Insert (pVectorHandle, PVector_End (pVectorHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PVector_Erase (pVectorHandle, PVector_Begin (pVectorHandle));
    PVectorSuite_Helper_CheckSize (size - 1);
    PVectorSuite_Helper_IterateCheckNaturalOrdered ();
    PVectorSuite_Helper_ItemAtCheckNaturalOrdered (size - 1);
}

void PVectorSuite_EraseEnd ()
{
    uint size = 10;
    for (int number = 0; number < size; ++number)
    {
        int *value = malloc (sizeof (int));
        *value = number == size - 1 ? size - 2 : number;
        PVector_Insert (pVectorHandle, PVector_End (pVectorHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PVector_Erase (pVectorHandle, PVectorIterator_Previous (PVector_End (pVectorHandle)));
    PVectorSuite_Helper_CheckSize (size - 1);
    PVectorSuite_Helper_IterateCheckNaturalOrdered ();
    PVectorSuite_Helper_ItemAtCheckNaturalOrdered (size - 1);
}

void PVectorSuite_EraseMiddle ()
{
    uint size = 20;
    for (int number = 0; number < size; ++number)
    {
        int *value = malloc (sizeof (int));
        *value = number / 2;
        PVector_Insert (pVectorHandle, PVector_End (pVectorHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PVectorIterator iterator = PVector_Begin (pVectorHandle);
    for (uint index = 0; index < size; ++index)
    {
        if (index % 2 != 0)
        {
            iterator = PVector_Erase (pVectorHandle, iterator);
        }
        else
        {
            iterator = PVectorIterator_Next (iterator);
        }
    }

    PVectorSuite_Helper_CheckSize (size / 2);
    PVectorSuite_Helper_IterateCheckNaturalOrdered ();
    PVectorSuite_Helper_ItemAtCheckNaturalOrdered (size / 2);
}

void RegisterPVectorSuite ()
{
    CU_CI_DEFINE_SUITE ("PVector", NULL, NULL, PVectorSuite_Setup, PVectorSuite_Teardown);
    CUNIT_CI_TEST (PVectorSuite_EmptyCreateFree);
    CUNIT_CI_TEST (PVectorSuite_EmptyBeginEndIterator);

    CUNIT_CI_TEST (PVectorSuite_InsertBackAndIterateInitialCapacity);
    CUNIT_CI_TEST (PVectorSuite_InsertBeginAndIterateInitialCapacity);
    CUNIT_CI_TEST (PVectorSuite_InsertMiddleAndIterateInitialCapacity);

    CUNIT_CI_TEST (PVectorSuite_InsertBackAndIterateExtendedCapacity);
    CUNIT_CI_TEST (PVectorSuite_InsertBeginAndIterateExtendedCapacity);
    CUNIT_CI_TEST (PVectorSuite_InsertMiddleAndIterateExtendedCapacity);

    CUNIT_CI_TEST (PVectorSuite_EraseBegin);
    CUNIT_CI_TEST (PVectorSuite_EraseEnd);
    CUNIT_CI_TEST (PVectorSuite_EraseMiddle);
}
