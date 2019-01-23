#include "TestPHashMap.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CContainers/PHashMap.h>
#include <CContainers/Utils.h>

#define INITIAL_BUCKETS_COUNT 10
#define MAX_LOAD 5

PHashMapHandle pHashMapHandle;

ulint HashKey (void *key)
{
    return (ulint) *(int *) key;
}

lint KeyCompare (void *first, void *second)
{
    return *(int *) first - *(int *) second;
}

static void PHashMapSuite_Helper_CheckSize (uint size)
{
    if (size != PHashMap_Size (pHashMapHandle))
    {
        printf ("\n        Size (e/a): %d, %d.", size, (int) PHashMap_Size (pHashMapHandle));
        CU_FAIL_FATAL ("Expected and actual values are not equal!");
    }
}

static void PHashMapSuite_Helper_FillHashMap (uint count)
{
    for (uint index = 0; index < count; ++index)
    {
        int *key = malloc (sizeof (int));
        int *value = malloc (sizeof (int));

        *key = index;
        *value = index;

        PHashMap_Insert (pHashMapHandle, key, value);
    }
}

static void PHashMapSuite_Helper_ContainsTest (uint count)
{
    PHashMapSuite_Helper_FillHashMap (count);
    PHashMapSuite_Helper_CheckSize (count);

    for (int index = 0; index < count * 2; ++index)
    {
        char expected = index < count;
        char actual = PHashMap_ContainsKey (pHashMapHandle, &index);

        if (expected != actual)
        {
            printf ("\n    Contains key %d? (e/a): %d/%d.", index, expected, actual);
            CU_FAIL ("Incorrect map content check!";)
        }
    }
}

static void PHashMapSuite_Helper_GetValueTest (uint count)
{
    PHashMapSuite_Helper_FillHashMap (count);
    PHashMapSuite_Helper_CheckSize (count);

    for (int index = 0; index < count; ++index)
    {
        int value = **(int **) PHashMap_GetValue (pHashMapHandle, &index);

        if (index != value)
        {
            printf ("\n    Get value %d = %d.", index, value);
            CU_FAIL ("Incorrect map get value check!";)
        }
    }
}

static void PHashMapSuite_Helper_EraseTest (uint count)
{
    PHashMapSuite_Helper_FillHashMap (count);
    PHashMapSuite_Helper_CheckSize (count);

    int index = count / 2;
    PHashMap_Erase (pHashMapHandle, &index, ContainerCallback_Free, ContainerCallback_Free);
    PHashMapSuite_Helper_CheckSize (count - 1);

    char expected = 0;
    char actual = PHashMap_ContainsKey (pHashMapHandle, &index);

    if (expected != actual)
    {
        printf ("\n    Contains key %d after erase? (e/a): %d/%d.", index, expected, actual);
        CU_FAIL ("Incorrect map content check!";)
    }
}

static void PHashMapSuite_Setup ()
{
    pHashMapHandle = PHashMap_Create (INITIAL_BUCKETS_COUNT, MAX_LOAD, HashKey, KeyCompare);
}

static void PHashMapSuite_Teardown ()
{
    PHashMap_Destruct (pHashMapHandle, ContainerCallback_Free, ContainerCallback_Free);
}

static void PHashMapSuite_ContainsNoRebucketing ()
{
    PHashMapSuite_Helper_ContainsTest (MAX_LOAD - 1);
}

static void PHashMapSuite_Contains ()
{
    PHashMapSuite_Helper_ContainsTest (MAX_LOAD * INITIAL_BUCKETS_COUNT + 1);
}

static void PHashMapSuite_GetValueNoRebucketing ()
{
    PHashMapSuite_Helper_GetValueTest (MAX_LOAD - 1);
}

static void PHashMapSuite_GetValue ()
{
    PHashMapSuite_Helper_GetValueTest (MAX_LOAD * INITIAL_BUCKETS_COUNT + 1);
}

static void PHashMapSuite_EraseNoRebucketing ()
{
    PHashMapSuite_Helper_EraseTest (MAX_LOAD - 1);
}

static void PHashMapSuite_Erase ()
{
    PHashMapSuite_Helper_EraseTest (MAX_LOAD * INITIAL_BUCKETS_COUNT + 1);
}

void RegisterPHashMapSuite ()
{
    CU_CI_DEFINE_SUITE ("PHashMap", NULL, NULL, PHashMapSuite_Setup, PHashMapSuite_Teardown);
    CUNIT_CI_TEST (PHashMapSuite_ContainsNoRebucketing);
    CUNIT_CI_TEST (PHashMapSuite_Contains);

    CUNIT_CI_TEST (PHashMapSuite_GetValueNoRebucketing);
    CUNIT_CI_TEST (PHashMapSuite_GetValue);

    CUNIT_CI_TEST (PHashMapSuite_EraseNoRebucketing);
    CUNIT_CI_TEST (PHashMapSuite_Erase);
}
