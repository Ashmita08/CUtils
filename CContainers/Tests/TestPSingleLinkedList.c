#include "TestPSingleLinkedList.h"
#include <stdlib.h>

#include <cunit/CUnit.h>
#include <cunit/CUnitCI.h>

#include <CContainers/PSingleLinkedList.h>
#include <CContainers/Utils.h>

#define TEST_LIST_SIZE 10
static PSingleLinkedListHandle pSingleLinkedListHandle;

void PSingleLinkedListSuite_Helper_CheckSize (uint size)
{
    if (size != PSingleLinkedList_Size (pSingleLinkedListHandle))
    {
        printf ("\n        Size (e/a): %d, %d.", size, PSingleLinkedList_Size (pSingleLinkedListHandle));
        CU_FAIL_FATAL ("Expected and actual values are not equal!");
    }
}

static int CheckIterationOrderCallback_number = 0;

void CheckIterationOrderCallback (void **item)
{
    int number = **((int **) item);
    if (CheckIterationOrderCallback_number != number)
    {
        printf ("\nValue at %d (e/a): %d, %d.", CheckIterationOrderCallback_number,
                CheckIterationOrderCallback_number, number);
    }

    ++CheckIterationOrderCallback_number;
}

void PSingleLinkedListSuite_Helper_IterateCheckNaturalOrdered ()
{
    CheckIterationOrderCallback_number = 0;
    PSingleLinkedListIterator_ForEach (PSingleLinkedList_Begin (pSingleLinkedListHandle),
            PSingleLinkedList_End (pSingleLinkedListHandle), CheckIterationOrderCallback);
}

void PSingleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (uint size)
{
    for (uint index = 0; index < size; ++index)
    {
        PSingleLinkedListIterator iterator = PSingleLinkedList_At (pSingleLinkedListHandle, index);
        if (CContainers_GetLastError () != 0)
        {
            printf ("\n        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            CU_FAIL_FATAL ("CContainers internal error!");
        }

        int value = **((int **) PSingleLinkedListIterator_ValueAt (iterator));
        if (value != index)
        {
            printf ("\n        Value at %d (e/a): %d, %d.", index, index, value);
            CU_FAIL ("Vector content is incorrect!");
        }
    }
}

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

    PSingleLinkedListSuite_Helper_CheckSize (TEST_LIST_SIZE);
    PSingleLinkedListSuite_Helper_IterateCheckNaturalOrdered ();
    PSingleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (TEST_LIST_SIZE);
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

    PSingleLinkedListSuite_Helper_CheckSize (2);
    PSingleLinkedListSuite_Helper_IterateCheckNaturalOrdered ();
    PSingleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (2);
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

    PSingleLinkedListSuite_Helper_CheckSize (TEST_LIST_SIZE);
    PSingleLinkedListSuite_Helper_IterateCheckNaturalOrdered ();
    PSingleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (TEST_LIST_SIZE);
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
    PSingleLinkedListSuite_Helper_CheckSize (TEST_LIST_SIZE - 1);
    PSingleLinkedListSuite_Helper_IterateCheckNaturalOrdered ();
    PSingleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (TEST_LIST_SIZE - 1);
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

    PSingleLinkedListSuite_Helper_CheckSize (TEST_LIST_SIZE - 1);
    PSingleLinkedListSuite_Helper_IterateCheckNaturalOrdered ();
    PSingleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (TEST_LIST_SIZE - 1);
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

    PSingleLinkedListSuite_Helper_CheckSize (TEST_LIST_SIZE - 1);
    PSingleLinkedListSuite_Helper_IterateCheckNaturalOrdered ();
    PSingleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (TEST_LIST_SIZE - 1);
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

    PSingleLinkedListSuite_Helper_CheckSize (TEST_LIST_SIZE);
    PSingleLinkedListSuite_Helper_IterateCheckNaturalOrdered ();
    PSingleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (TEST_LIST_SIZE);
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

    PSingleLinkedListSuite_Helper_CheckSize (TEST_LIST_SIZE - 1);
    PSingleLinkedListSuite_Helper_IterateCheckNaturalOrdered ();
    PSingleLinkedListSuite_Helper_ItemAtCheckNaturalOrdered (TEST_LIST_SIZE - 1);
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
