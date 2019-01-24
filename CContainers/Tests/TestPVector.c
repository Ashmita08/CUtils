#include "TestPVector.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CContainers/PVector.h>
#include <CContainers/Utils.h>
#include "Utils.h"

#define INITIAL_CAPACITY 5
PVectorHandle pVectorHandle;

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

    SizeChecker (pVectorHandle, PVector_AsISizedContainer (), size, 1);
    IterationNaturalOrderChecker (pVectorHandle, PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
    ItemAtNaturalOrderChecker (pVectorHandle, PVector_AsISizedContainer (),
            PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
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

    SizeChecker (pVectorHandle, PVector_AsISizedContainer (), size, 1);
    IterationNaturalOrderChecker (pVectorHandle, PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
    ItemAtNaturalOrderChecker (pVectorHandle, PVector_AsISizedContainer (),
            PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
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

    SizeChecker (pVectorHandle, PVector_AsISizedContainer (), size, 1);
    IterationNaturalOrderChecker (pVectorHandle, PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
    ItemAtNaturalOrderChecker (pVectorHandle, PVector_AsISizedContainer (),
            PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
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
    SizeChecker (pVectorHandle, PVector_AsISizedContainer (), size - 1, 1);
    IterationNaturalOrderChecker (pVectorHandle, PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
    ItemAtNaturalOrderChecker (pVectorHandle, PVector_AsISizedContainer (),
            PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
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
    SizeChecker (pVectorHandle, PVector_AsISizedContainer (), size - 1, 1);
    IterationNaturalOrderChecker (pVectorHandle, PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
    ItemAtNaturalOrderChecker (pVectorHandle, PVector_AsISizedContainer (),
            PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
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

    SizeChecker (pVectorHandle, PVector_AsISizedContainer (), size / 2, 1);
    IterationNaturalOrderChecker (pVectorHandle, PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
    ItemAtNaturalOrderChecker (pVectorHandle, PVector_AsISizedContainer (),
            PVector_AsIIterableContainer (), PVectorIterator_AsIOneDirectionIterator (), 0);
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
