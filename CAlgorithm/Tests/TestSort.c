#include "TestSort.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CAlgorithm/Sort.h>
#include <CContainers/PVector.h>
#include <CContainers/Utils.h>

#define FIRST_TEST_SIZE 7
int FirstTest_Input [] = {1, 2, 3, 4, 5, 6, 7};
int FirstTest_Output [] = {1, 2, 3, 4, 5, 6, 7};

#define SECOND_TEST_SIZE 7
int SecondTest_Input [] = {7, 6, 5, 4, 3, 2, 1};
int SecondTest_Output [] = {1, 2, 3, 4, 5, 6, 7};

#define THIRD_TEST_SIZE 8
int ThirdTest_Input [] = {34, 12, 31, 66, 98, 45, 23, 11};
int ThirdTest_Output [] = {11, 12, 23, 31, 34, 45, 66, 98};

#define FOURTH_TEST_SIZE 10
int FourthTest_Input [] = {1, 1, 1, 2, 3, 4, 4, 5, 6, 7};
int FourthTest_Output [] = {1, 1, 1, 2, 3, 4, 4, 5, 6, 7};

#define FIFTH_TEST_SIZE 10
int FifthTest_Input [] = {7, 7, 7, 6, 5, 5, 4, 3, 2, 1};
int FifthTest_Output [] = {1, 2, 3, 4, 5, 5, 6, 7, 7, 7};

#define SIXTH_TEST_SIZE 11
int SixthTest_Input [] = {34, 12, 34, 31, 66, 11, 45, 98, 45, 23, 11};
int SixthTest_Output [] = {11, 11, 12, 23, 31, 34, 34, 45, 45, 66, 98};

static lint Comparator (const void *first, const void *second)
{
    return (lint) (*(const int *) second - *(const int *) first);
}

static void SortSuite_Helper_Test (uint size, int *input, const int *expectedOutput)
{
    PVectorHandle vector = PVector_Create (size);
    for (uint index = 0; index < size; ++index)
    {
        PVector_Insert (vector, PVector_End (vector), input + index);
    }

    HeapSort (vector, PVector_AsISizedContainer (), PVector_AsIIterableContainer (),
            PVectorIterator_AsIBiDirectionalIterator (), PVector_AsIMutableContainer (), Comparator);

    for (uint index = 0; index < size; ++index)
    {
        int actual = **(int **) PVectorIterator_ValueAt (PVector_At (vector, index));
        int expected = expectedOutput [index];
        
        if (actual != expected)
        {
            printf ("\n    Error at index %d (e/a): %d, %d.", index, expected, actual);
            CU_FAIL ("Incorrect order found!");
        }
    }

    PVector_Destruct (vector, ContainerCallback_NoAction);
}

static void SortSuite_Setup ()
{

}

static void SortSuite_Teardown ()
{

}

static void SortSuite_First ()
{
    SortSuite_Helper_Test (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void SortSuite_Second ()
{
    SortSuite_Helper_Test (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void SortSuite_Third ()
{
    SortSuite_Helper_Test (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void SortSuite_Fourth ()
{
    SortSuite_Helper_Test (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void SortSuite_Fifth ()
{
    SortSuite_Helper_Test (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void SortSuite_Sixth ()
{
    SortSuite_Helper_Test (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

void RegisterSortSuite ()
{
    CU_CI_DEFINE_SUITE ("Sort", NULL, NULL, SortSuite_Setup, SortSuite_Teardown);
    CUNIT_CI_TEST (SortSuite_First);
    CUNIT_CI_TEST (SortSuite_Second);
    CUNIT_CI_TEST (SortSuite_Third);
    CUNIT_CI_TEST (SortSuite_Fourth);
    CUNIT_CI_TEST (SortSuite_Fifth);
    CUNIT_CI_TEST (SortSuite_Sixth);
}
