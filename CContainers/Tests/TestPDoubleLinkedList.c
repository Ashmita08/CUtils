#include "TestPDoubleLinkedList.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CContainers/PDoubleLinkedList.h>
#include <CContainers/Utils.h>

#define TEST_LIST_SIZE 10
static PDoubleLinkedListHandle pDoubleLinkedListHandle;

// TODO: Maybe use container interfaces to move all helpers to separate compile object?
void PDoubleLinkedListSuite_Helper_CheckSize (uint size)
{
    if (size != PDoubleLinkedList_Size (pDoubleLinkedListHandle))
    {
        printf ("\n        Size (e/a): %d, %d.", size, PDoubleLinkedList_Size (pDoubleLinkedListHandle));
        CU_FAIL_FATAL ("Expected and actual values are not equal!");
    }
}

static int CheckIterationOrderCallback_number = 0;

void CheckIterationOrderAscendingCallback (void **item)
{
    int number = **((int **) item);
    if (CheckIterationOrderCallback_number != number)
    {
        printf ("\nValue at %d (e/a): %d, %d.", CheckIterationOrderCallback_number,
                CheckIterationOrderCallback_number, number);
    }

    ++CheckIterationOrderCallback_number;
}

void CheckIterationOrderDescendingCallback (void **item)
{
    int number = **((int **) item);
    if (CheckIterationOrderCallback_number != number)
    {
        printf ("\nValue at %d (e/a): %d, %d.", CheckIterationOrderCallback_number,
                CheckIterationOrderCallback_number, number);
    }

    --CheckIterationOrderCallback_number;
}

void PDoubleLinkedListSuite_Helper_IterateCheckNaturalOrdered ()
{
    CheckIterationOrderCallback_number = 0;
    PDoubleLinkedListIterator_ForEach (PDoubleLinkedList_Begin (pDoubleLinkedListHandle),
            PDoubleLinkedList_End (pDoubleLinkedListHandle),
            CheckIterationOrderAscendingCallback);
}

void PDoubleLinkedListSuite_Helper_IterateCheckNaturalOrderedReversedForEach (uint size)
{
    CheckIterationOrderCallback_number = size - 1;
    PDoubleLinkedListIterator_ForEachReversed (PDoubleLinkedList_Begin (pDoubleLinkedListHandle),
            PDoubleLinkedListIterator_Previous (PDoubleLinkedList_End (pDoubleLinkedListHandle)),
            CheckIterationOrderDescendingCallback);
}

void PDoubleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (uint size)
{
    for (uint index = 0; index < size; ++index)
    {
        PDoubleLinkedListIterator iterator = PDoubleLinkedList_At (pDoubleLinkedListHandle, index);
        if (CContainers_GetLastError () != 0)
        {
            printf ("\n        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }

        int value = **((int **) PDoubleLinkedListIterator_ValueAt (iterator));
        if (value != index)
        {
            printf ("\n        Value at %d (e/a): %d, %d.", index, index, value);
            CU_FAIL ("Vector content is incorrect!");
        }
    }
}

void PDoubleLinkedListSuite_Helper_RunChecks (uint size)
{
    PDoubleLinkedListSuite_Helper_CheckSize (size);
    PDoubleLinkedListSuite_Helper_IterateCheckNaturalOrdered ();
    PDoubleLinkedListSuite_Helper_IterateCheckNaturalOrderedReversedForEach (size);
    PDoubleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (size);
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
