#include "TestPDoubleLinkedList.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CContainers/PDoubleLinkedList.h>
#include <CContainers/Utils.h>
#include "Utils.h"

#define TEST_LIST_SIZE 10
static PDoubleLinkedListHandle pDoubleLinkedListHandle;

void PDoubleLinkedListSuite_Helper_RunChecks (uint size)
{
    SizeChecker (pDoubleLinkedListHandle, PDoubleLinkedList_AsISizedContainer (), size, 1);
    IterationNaturalOrderChecker (pDoubleLinkedListHandle,
            PDoubleLinkedList_AsIIterableContainer (), PDoubleLinkedListIterator_AsIOneDirectionIterator (), 0);

    IterationReversedNaturalOrderChecker (pDoubleLinkedListHandle, PDoubleLinkedList_AsISizedContainer (),
            PDoubleLinkedList_AsIIterableContainer (), PDoubleLinkedListIterator_AsIBiDirectionalIterator (), 0);

    ItemAtNaturalOrderChecker (pDoubleLinkedListHandle, PDoubleLinkedList_AsISizedContainer (),
            PDoubleLinkedList_AsIIterableContainer (), PDoubleLinkedListIterator_AsIOneDirectionIterator (), 0);
}

void PDoubleLinkedListSuite_Setup ()
{
    pDoubleLinkedListHandle = PDoubleLinkedList_Create ();
    CContainers_SetLastError (0);
}

void PDoubleLinkedListSuite_Teardown ()
{
    PDoubleLinkedList_Destruct (pDoubleLinkedListHandle, ContainerCallback_Free);
}

void PDoubleLinkedListSuite_InsertFrontAndIterate ()
{
    for (uint index = TEST_LIST_SIZE - 1; index < TEST_LIST_SIZE; --index)
    {
        int *value = malloc (sizeof (int));
        *value = index;
        PDoubleLinkedList_Insert (pDoubleLinkedListHandle, PDoubleLinkedList_Begin (pDoubleLinkedListHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PDoubleLinkedListSuite_Helper_RunChecks (TEST_LIST_SIZE);
}

void PDoubleLinkedListSuite_InsertBackAndIterate ()
{
    for (uint index = 0; index < TEST_LIST_SIZE; ++index)
    {
        int *value = malloc (sizeof (int));
        *value = index;
        PDoubleLinkedList_Insert (pDoubleLinkedListHandle, PDoubleLinkedList_End (pDoubleLinkedListHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PDoubleLinkedListSuite_Helper_RunChecks (TEST_LIST_SIZE);
}

void PDoubleLinkedListSuite_InsertMiddleAndIterate ()
{
    for (uint index = 0; index < TEST_LIST_SIZE; ++index)
    {
        int *value = malloc (sizeof (int));
        *value = index < TEST_LIST_SIZE / 2 ? index : index + 1;

        if (index == TEST_LIST_SIZE - 1)
        {
            *value = TEST_LIST_SIZE / 2;
            PDoubleLinkedList_Insert (pDoubleLinkedListHandle,
                    PDoubleLinkedList_At (pDoubleLinkedListHandle, TEST_LIST_SIZE / 2), value);
        }
        else
        {
            PDoubleLinkedList_Insert (pDoubleLinkedListHandle, PDoubleLinkedList_End (pDoubleLinkedListHandle), value);
        }

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PDoubleLinkedListSuite_Helper_RunChecks (TEST_LIST_SIZE);
}

void PDoubleLinkedListSuite_EraseFrontAndIterate ()
{
    for (uint index = 0; index < TEST_LIST_SIZE + 1; ++index)
    {
        int *value = malloc (sizeof (int));
        *value = index != 0 ? index - 1 : index;
        PDoubleLinkedList_Insert (pDoubleLinkedListHandle, PDoubleLinkedList_End (pDoubleLinkedListHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PDoubleLinkedList_Erase (pDoubleLinkedListHandle, PDoubleLinkedList_Begin (pDoubleLinkedListHandle));
    PDoubleLinkedListSuite_Helper_RunChecks (TEST_LIST_SIZE);
}

void PDoubleLinkedListSuite_EraseBackAndIterate ()
{
    for (uint index = 0; index < TEST_LIST_SIZE + 1; ++index)
    {
        int *value = malloc (sizeof (int));
        *value = index != TEST_LIST_SIZE ? index : index - 1;
        PDoubleLinkedList_Insert (pDoubleLinkedListHandle, PDoubleLinkedList_End (pDoubleLinkedListHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PDoubleLinkedList_Erase (pDoubleLinkedListHandle,
            PDoubleLinkedListIterator_Previous (PDoubleLinkedList_End (pDoubleLinkedListHandle)));
    PDoubleLinkedListSuite_Helper_RunChecks (TEST_LIST_SIZE);
}

void PDoubleLinkedListSuite_EraseMiddleAndIterate ()
{
    for (uint index = 0; index < TEST_LIST_SIZE + 1; ++index)
    {
        int *value = malloc (sizeof (int));
        *value = index < TEST_LIST_SIZE / 2 ? index : index - 1;
        PDoubleLinkedList_Insert (pDoubleLinkedListHandle, PDoubleLinkedList_End (pDoubleLinkedListHandle), value);

        if (CContainers_GetLastError () != 0)
        {
            printf ("        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }
    }

    PDoubleLinkedList_Erase (pDoubleLinkedListHandle,
            PDoubleLinkedList_At (pDoubleLinkedListHandle, TEST_LIST_SIZE / 2));
    PDoubleLinkedListSuite_Helper_RunChecks (TEST_LIST_SIZE);
}

void RegisterPDoubleLinkedListSuite ()
{
    CU_CI_DEFINE_SUITE ("PDoubleLinkedList", NULL, NULL, PDoubleLinkedListSuite_Setup, PDoubleLinkedListSuite_Teardown);
    CUNIT_CI_TEST (PDoubleLinkedListSuite_InsertFrontAndIterate);
    CUNIT_CI_TEST (PDoubleLinkedListSuite_InsertBackAndIterate);
    CUNIT_CI_TEST (PDoubleLinkedListSuite_InsertMiddleAndIterate);

    CUNIT_CI_TEST (PDoubleLinkedListSuite_EraseFrontAndIterate);
    CUNIT_CI_TEST (PDoubleLinkedListSuite_EraseBackAndIterate);
    CUNIT_CI_TEST (PDoubleLinkedListSuite_EraseMiddleAndIterate);
}
