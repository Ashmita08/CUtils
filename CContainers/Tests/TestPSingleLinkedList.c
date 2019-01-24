#include "TestPSingleLinkedList.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CContainers/PSingleLinkedList.h>
#include <CContainers/Utils.h>
#include "Utils.h"

#define TEST_LIST_SIZE 10
static PSingleLinkedListHandle pSingleLinkedListHandle;

void PSingleLinkedListSuite_Setup ()
{
    pSingleLinkedListHandle = PSingleLinkedList_Create ();
    CContainers_SetLastError (0);
}

void PSingleLinkedListSuite_Teardown ()
{
    PSingleLinkedList_Destruct (pSingleLinkedListHandle, ContainerCallback_Free);
}

void PSingleLinkedListSuite_InsertFrontAndIterate ()
{
    for (uint index = TEST_LIST_SIZE - 1; index < TEST_LIST_SIZE; --index)
    {
        int *value = malloc (sizeof (int));
        *value = index;
        PSingleLinkedList_InsertFront (pSingleLinkedListHandle, value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    SizeChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (), TEST_LIST_SIZE, 1);
    IterationNaturalOrderChecker (pSingleLinkedListHandle,
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);

    ItemAtNaturalOrderChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (),
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);
}

void PSingleLinkedListSuite_InsertLastAndIterate ()
{
    for (uint index = 0; index < 2; ++index)
    {
        int *value = malloc (sizeof (int));
        *value = index;
        PSingleLinkedList_InsertAfter (pSingleLinkedListHandle,
                PSingleLinkedList_Begin (pSingleLinkedListHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    SizeChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (), 2, 1);
    IterationNaturalOrderChecker (pSingleLinkedListHandle,
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);

    ItemAtNaturalOrderChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (),
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);
}

void PSingleLinkedListSuite_InsertMiddleAndIterate ()
{
    for (uint index = TEST_LIST_SIZE - 1; index < TEST_LIST_SIZE; --index)
    {
        if (index == TEST_LIST_SIZE / 3)
        {
            continue;
        }

        int *value = malloc (sizeof (int));
        *value = index;
        PSingleLinkedList_InsertFront (pSingleLinkedListHandle, value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    int *value = malloc (sizeof (int));
    *value = TEST_LIST_SIZE / 3;
    PSingleLinkedList_InsertAfter (pSingleLinkedListHandle,
            PSingleLinkedList_At (pSingleLinkedListHandle, TEST_LIST_SIZE / 3 - 1), value);

    SizeChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (), TEST_LIST_SIZE, 1);
    IterationNaturalOrderChecker (pSingleLinkedListHandle,
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);

    ItemAtNaturalOrderChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (),
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);
}

void PSingleLinkedListSuite_EraseFirstAndIterate ()
{
    for (uint index = TEST_LIST_SIZE - 1; index < TEST_LIST_SIZE; --index)
    {
        int *value = malloc (sizeof (int));
        *value = index - 1;
        PSingleLinkedList_InsertFront (pSingleLinkedListHandle, value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PSingleLinkedList_Erase (pSingleLinkedListHandle, PSingleLinkedList_Begin (pSingleLinkedListHandle));
    SizeChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (), TEST_LIST_SIZE - 1, 1);
    IterationNaturalOrderChecker (pSingleLinkedListHandle,
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);

    ItemAtNaturalOrderChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (),
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);
}

void PSingleLinkedListSuite_EraseLastAndIterate ()
{
    for (uint index = TEST_LIST_SIZE - 1; index < TEST_LIST_SIZE; --index)
    {
        int *value = malloc (sizeof (int));
        *value = index;
        PSingleLinkedList_InsertFront (pSingleLinkedListHandle, value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PSingleLinkedList_Erase (pSingleLinkedListHandle,
            PSingleLinkedList_At (pSingleLinkedListHandle, TEST_LIST_SIZE - 1));

    SizeChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (), TEST_LIST_SIZE - 1, 1);
    IterationNaturalOrderChecker (pSingleLinkedListHandle,
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);

    ItemAtNaturalOrderChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (),
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);
}

void PSingleLinkedListSuite_EraseMiddleAndIterate ()
{
    for (uint index = TEST_LIST_SIZE - 1; index < TEST_LIST_SIZE; --index)
    {
        int *value = malloc (sizeof (int));
        *value = index > TEST_LIST_SIZE / 2 ? index - 1 : index;
        PSingleLinkedList_InsertFront (pSingleLinkedListHandle, value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PSingleLinkedList_Erase (pSingleLinkedListHandle,
            PSingleLinkedList_At (pSingleLinkedListHandle, TEST_LIST_SIZE / 2));

    SizeChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (), TEST_LIST_SIZE - 1, 1);
    IterationNaturalOrderChecker (pSingleLinkedListHandle,
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);

    ItemAtNaturalOrderChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (),
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);
}

void PSingleLinkedListSuite_EraseNextAndIterateNothing ()
{
    for (uint index = TEST_LIST_SIZE - 1; index < TEST_LIST_SIZE; --index)
    {
        int *value = malloc (sizeof (int));
        *value = index;
        PSingleLinkedList_InsertFront (pSingleLinkedListHandle, value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PSingleLinkedList_EraseNext (pSingleLinkedListHandle,
            PSingleLinkedList_At (pSingleLinkedListHandle, TEST_LIST_SIZE - 1));

    SizeChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (), TEST_LIST_SIZE, 1);
    IterationNaturalOrderChecker (pSingleLinkedListHandle,
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);

    ItemAtNaturalOrderChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (),
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);
}

void PSingleLinkedListSuite_EraseNextAndIterate ()
{
    for (uint index = TEST_LIST_SIZE - 1; index < TEST_LIST_SIZE; --index)
    {
        int *value = malloc (sizeof (int));
        *value = index > TEST_LIST_SIZE / 2 ? index - 1 : index;
        PSingleLinkedList_InsertFront (pSingleLinkedListHandle, value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PSingleLinkedList_EraseNext (pSingleLinkedListHandle,
            PSingleLinkedList_At (pSingleLinkedListHandle, TEST_LIST_SIZE / 2 - 1));

    SizeChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (), TEST_LIST_SIZE - 1, 1);
    IterationNaturalOrderChecker (pSingleLinkedListHandle,
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);

    ItemAtNaturalOrderChecker (pSingleLinkedListHandle, PSingleLinkedList_AsISizedContainer (),
            PSingleLinkedList_AsIIterableContainer (), PSingleLinkedListIterator_AsIOneDirectionIterator (), 0);
}

void RegisterPSingleLinkedListSuite ()
{
    CU_CI_DEFINE_SUITE ("PSingleLinkedList", NULL, NULL, PSingleLinkedListSuite_Setup, PSingleLinkedListSuite_Teardown);
    CUNIT_CI_TEST (PSingleLinkedListSuite_InsertFrontAndIterate);
    CUNIT_CI_TEST (PSingleLinkedListSuite_InsertLastAndIterate);
    CUNIT_CI_TEST (PSingleLinkedListSuite_InsertMiddleAndIterate);

    CUNIT_CI_TEST (PSingleLinkedListSuite_EraseFirstAndIterate);
    CUNIT_CI_TEST (PSingleLinkedListSuite_EraseLastAndIterate);
    CUNIT_CI_TEST (PSingleLinkedListSuite_EraseMiddleAndIterate);

    CUNIT_CI_TEST (PSingleLinkedListSuite_EraseNextAndIterateNothing);
    CUNIT_CI_TEST (PSingleLinkedListSuite_EraseNextAndIterate);
}
