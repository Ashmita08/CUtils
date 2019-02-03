#include "TestPODVector.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CContainers/PODVector.h>
#include <CContainers/Utils.h>

#define INITIAL_CAPACITY 5
#define ELEMENT_SIZE (sizeof (int))
static PODVectorHandle podVectorHandle;

static void SizeChecker (ulint expected)
{
    if (expected != PODVector_Size (podVectorHandle))
    {
        printf ("\n        Size (e/a): %d, %d.", (int) expected, (int) PODVector_Size (podVectorHandle));
        CU_FAIL_FATAL ("Expected and actual values are not equal!");
    }
}

static void CapacityChecker (ulint expected)
{
    if (expected != PODVector_Capacity (podVectorHandle))
    {
        printf ("\n        Capacity (e/a): %d, %d.", (int) expected, (int) PODVector_Capacity (podVectorHandle));
        CU_FAIL_FATAL ("Expected and actual values are not equal!");
    }
}

static void IterationNaturalOrderChecker ()
{
    int number = 0;
    char *position = PODVector_Begin (podVectorHandle);

    while (position != PODVector_End (podVectorHandle))
    {
        int value = *(int *) position;
        if (value != number)
        {
            printf ("\n        Value at %d (e/a): %d, %d.", number, number, value);
            CU_FAIL ("Container content is incorrect!");
        }

        position += ELEMENT_SIZE;
        ++number;
    }
}

static void ItemAtNaturalOrderChecker ()
{
    for (ulint index = 0; index < PODVector_Size (podVectorHandle); ++index)
    {
        char *position = PODVector_At (podVectorHandle, index);
        if (CContainers_GetLastError () != 0)
        {
            printf ("\n        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL ("CContainers internal error!");
        }

        int value = *(int *) position;
        if (value != index)
        {
            printf ("\n        Value at %d (e/a): %d, %d.", (int) index, (int) index, value);
            CU_FAIL ("Container content is incorrect!");
        }
    }
}

void PODVectorSuite_Helper_InsertBackAndIterate (ulint size, uint capacity)
{
    for (int number = 0; number < size; ++number)
    {
        PODVector_Insert (podVectorHandle, (ulint) number, (char *) &number);
        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    SizeChecker (size);
    CapacityChecker (capacity);
    IterationNaturalOrderChecker ();
    ItemAtNaturalOrderChecker ();
}

void PODVectorSuite_Helper_InsertBeginAndIterate (ulint size, uint capacity)
{
    for (int number = (int) size - 1; number >= 0; --number)
    {
        PODVector_Insert (podVectorHandle, 0, (char *) &number);
        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    SizeChecker (size);
    CapacityChecker (capacity);
    IterationNaturalOrderChecker ();
    ItemAtNaturalOrderChecker ();
}

void PODVectorSuite_Helper_InsertMiddleAndIterate (ulint size, uint capacity)
{
    if (size % 2 == 0)
    {
        ++size;
    }

    for (int number = 0; number < size; number += 2)
    {
        PODVector_Insert (podVectorHandle, PODVector_Size (podVectorHandle), (char *) &number);
        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    for (int number = 1; number < size; number += 2)
    {
        PODVector_Insert (podVectorHandle, (ulint) number, (char *) &number);
    }

    SizeChecker (size);
    CapacityChecker (capacity);
    IterationNaturalOrderChecker ();
    ItemAtNaturalOrderChecker ();
}

void PODVectorSuite_Setup ()
{
    podVectorHandle = PODVector_Create (INITIAL_CAPACITY, ELEMENT_SIZE);
    CContainers_SetLastError (0);
}

void PODVectorSuite_Teardown ()
{
    PODVector_Destruct (podVectorHandle, ContainerCallback_NoAction);
}

void PODVectorSuite_EmptyCreateFree ()
{

}

void PODVectorSuite_EmptyBeginEndIterator ()
{
    if (PODVector_Begin (podVectorHandle) != PODVector_End (podVectorHandle))
    {
        CU_FAIL_FATAL ("Begin and end must be equal");
    }
}

void PODVectorSuite_InsertBackAndIterateInitialCapacity ()
{
    PODVectorSuite_Helper_InsertBackAndIterate (INITIAL_CAPACITY, INITIAL_CAPACITY);
}

void PODVectorSuite_InsertBeginAndIterateInitialCapacity ()
{
    PODVectorSuite_Helper_InsertBeginAndIterate (INITIAL_CAPACITY, INITIAL_CAPACITY);
}

void PODVectorSuite_InsertMiddleAndIterateInitialCapacity ()
{
    PODVectorSuite_Helper_InsertMiddleAndIterate (INITIAL_CAPACITY, INITIAL_CAPACITY);
}

void PODVectorSuite_InsertBackAndIterateExtendedCapacity ()
{
    PODVectorSuite_Helper_InsertBackAndIterate (INITIAL_CAPACITY * 2, INITIAL_CAPACITY * 2);
}

void PODVectorSuite_InsertBeginAndIterateExtendedCapacity ()
{
    PODVectorSuite_Helper_InsertBeginAndIterate (INITIAL_CAPACITY * 2, INITIAL_CAPACITY * 2);
}

void PODVectorSuite_InsertMiddleAndIterateExtendedCapacity ()
{
    PODVectorSuite_Helper_InsertMiddleAndIterate (INITIAL_CAPACITY * 2, INITIAL_CAPACITY * 4);
}

void PODVectorSuite_EraseBegin ()
{
    ulint size = 10;
    for (int number = 0; number < size + 1; ++number)
    {
        int value = number == 0 ? 0 : number - 1;
        PODVector_Insert (podVectorHandle, PODVector_Size (podVectorHandle), (char *) &value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PODVector_Erase (podVectorHandle, 0);
    SizeChecker (size);
    IterationNaturalOrderChecker ();
    ItemAtNaturalOrderChecker ();
}

void PODVectorSuite_EraseEnd ()
{
    ulint size = 10;
    for (int number = 0; number < size; ++number)
    {
        int value = (int) (number == size - 1 ? size - 2 : number);
        PODVector_Insert (podVectorHandle, PODVector_Size (podVectorHandle), (char *) &value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PODVector_Erase (podVectorHandle, PODVector_Size (podVectorHandle) - 1);
    SizeChecker (size - 1);
    IterationNaturalOrderChecker ();
    ItemAtNaturalOrderChecker ();
}

void PODVectorSuite_EraseMiddle ()
{
    ulint size = 20;
    for (int number = 0; number < size; ++number)
    {
        int value = number / 2;
        PODVector_Insert (podVectorHandle, PODVector_Size (podVectorHandle), (char *) &value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    ulint removeIndex = 0;
    for (ulint index = 0; index < size; ++index)
    {
        if (index % 2 != 0)
        {
            PODVector_Erase (podVectorHandle, removeIndex);
        }
        else
        {
            ++removeIndex;
        }
    }

    SizeChecker (size / 2);
    IterationNaturalOrderChecker ();
    ItemAtNaturalOrderChecker ();
}

void PODVectorSuite_Realloc ()
{
    PODVectorSuite_Helper_InsertBackAndIterate (INITIAL_CAPACITY * 4, INITIAL_CAPACITY * 4);
    PODVector_Realloc (podVectorHandle, INITIAL_CAPACITY * 8);

    SizeChecker (INITIAL_CAPACITY * 4);
    CapacityChecker (INITIAL_CAPACITY * 8);
    IterationNaturalOrderChecker ();
    ItemAtNaturalOrderChecker ();

    PODVector_Realloc (podVectorHandle, INITIAL_CAPACITY * 2);
    SizeChecker (INITIAL_CAPACITY * 2);
    CapacityChecker (INITIAL_CAPACITY * 2);
    IterationNaturalOrderChecker ();
    ItemAtNaturalOrderChecker ();
}

void RegisterPODVectorSuite ()
{
    CU_CI_DEFINE_SUITE ("PODVector", NULL, NULL, PODVectorSuite_Setup, PODVectorSuite_Teardown);
    CUNIT_CI_TEST (PODVectorSuite_EmptyCreateFree);
    CUNIT_CI_TEST (PODVectorSuite_EmptyBeginEndIterator);

    CUNIT_CI_TEST (PODVectorSuite_InsertBackAndIterateInitialCapacity);
    CUNIT_CI_TEST (PODVectorSuite_InsertBeginAndIterateInitialCapacity);
    CUNIT_CI_TEST (PODVectorSuite_InsertMiddleAndIterateInitialCapacity);

    CUNIT_CI_TEST (PODVectorSuite_InsertBackAndIterateExtendedCapacity);
    CUNIT_CI_TEST (PODVectorSuite_InsertBeginAndIterateExtendedCapacity);
    CUNIT_CI_TEST (PODVectorSuite_InsertMiddleAndIterateExtendedCapacity);

    CUNIT_CI_TEST (PODVectorSuite_EraseBegin);
    CUNIT_CI_TEST (PODVectorSuite_EraseEnd);
    CUNIT_CI_TEST (PODVectorSuite_EraseMiddle);
    CUNIT_CI_TEST (PODVectorSuite_Realloc);
}
