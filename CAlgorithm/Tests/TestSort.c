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

void SortSuite_Helper_FillVector (PVectorHandle vector, uint size, int *input)
{
    for (uint index = 0; index < size; ++index)
    {
        PVector_Insert (vector, PVector_End (vector), input + index);
    }
}

void SortSuite_Helper_CheckVector (PVectorHandle vector, uint size, const int *expectedOutput)
{
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
}
static void SortSuite_Helper_HeapSortTest (uint size, int *input, const int *expectedOutput)
{
    PVectorHandle vector = PVector_Create (size);
    SortSuite_Helper_FillVector (vector, size, input);

    HeapSort (vector, PVector_AsISizedContainer (), PVector_AsIIterableContainer (),
            PVectorIterator_AsIBiDirectionalIterator (), PVector_AsIMutableContainer (), Comparator);

    SortSuite_Helper_CheckVector (vector, size, expectedOutput);
    PVector_Destruct (vector, ContainerCallback_NoAction);
}

static void SortSuite_Helper_MergeSortTest (uint size, int *input, const int *expectedOutput)
{
    PVectorHandle vector = PVector_Create (size);
    SortSuite_Helper_FillVector (vector, size, input);

    MergeSort (PVector_Begin (vector), PVector_End (vector), PVector_Size (vector),
            PVectorIterator_AsIOneDirectionIterator (), Comparator);

    SortSuite_Helper_CheckVector (vector, size, expectedOutput);
    PVector_Destruct (vector, ContainerCallback_NoAction);
}

static void SortSuite_Helper_InplaceMergeSortTest (uint size, int *input, const int *expectedOutput)
{
    PVectorHandle vector = PVector_Create (size);
    SortSuite_Helper_FillVector (vector, size, input);

    InplaceMergeSort (PVector_Begin (vector), PVector_End (vector), PVector_Size (vector),
            PVectorIterator_AsIBiDirectionalIterator (), Comparator);

    SortSuite_Helper_CheckVector (vector, size, expectedOutput);
    PVector_Destruct (vector, ContainerCallback_NoAction);
}

static void SortSuite_Helper_QuickSortTest (uint size, int *input, const int *expectedOutput)
{
    PVectorHandle vector = PVector_Create (size);
    SortSuite_Helper_FillVector (vector, size, input);

    QuickSort (PVector_Begin (vector), PVector_End (vector), PVector_Size (vector),
            PVectorIterator_AsIBiDirectionalIterator (), Comparator);

    SortSuite_Helper_CheckVector (vector, size, expectedOutput);
    PVector_Destruct (vector, ContainerCallback_NoAction);
}

static void SortSuite_Setup ()
{

}

static void SortSuite_Teardown ()
{

}

static void SortSuite_HeapFirst ()
{
    SortSuite_Helper_HeapSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void SortSuite_HeapSecond ()
{
    SortSuite_Helper_HeapSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void SortSuite_HeapThird ()
{
    SortSuite_Helper_HeapSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void SortSuite_HeapFourth ()
{
    SortSuite_Helper_HeapSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void SortSuite_HeapFifth ()
{
    SortSuite_Helper_HeapSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void SortSuite_HeapSixth ()
{
    SortSuite_Helper_HeapSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void SortSuite_MergeFirst ()
{
    SortSuite_Helper_MergeSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void SortSuite_MergeSecond ()
{
    SortSuite_Helper_MergeSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void SortSuite_MergeThird ()
{
    SortSuite_Helper_MergeSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void SortSuite_MergeFourth ()
{
    SortSuite_Helper_MergeSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void SortSuite_MergeFifth ()
{
    SortSuite_Helper_MergeSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void SortSuite_MergeSixth ()
{
    SortSuite_Helper_MergeSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void SortSuite_InplaceMergeFirst ()
{
    SortSuite_Helper_InplaceMergeSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void SortSuite_InplaceMergeSecond ()
{
    SortSuite_Helper_InplaceMergeSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void SortSuite_InplaceMergeThird ()
{
    SortSuite_Helper_InplaceMergeSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void SortSuite_InplaceMergeFourth ()
{
    SortSuite_Helper_InplaceMergeSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void SortSuite_InplaceMergeFifth ()
{
    SortSuite_Helper_InplaceMergeSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void SortSuite_InplaceMergeSixth ()
{
    SortSuite_Helper_InplaceMergeSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void SortSuite_QuickFirst ()
{
    SortSuite_Helper_QuickSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void SortSuite_QuickSecond ()
{
    SortSuite_Helper_QuickSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void SortSuite_QuickThird ()
{
    SortSuite_Helper_QuickSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void SortSuite_QuickFourth ()
{
    SortSuite_Helper_QuickSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void SortSuite_QuickFifth ()
{
    SortSuite_Helper_QuickSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void SortSuite_QuickSixth ()
{
    SortSuite_Helper_QuickSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

void RegisterSortSuite ()
{
    CU_CI_DEFINE_SUITE ("Sort", NULL, NULL, SortSuite_Setup, SortSuite_Teardown);
    CUNIT_CI_TEST (SortSuite_HeapFirst);
    CUNIT_CI_TEST (SortSuite_HeapSecond);
    CUNIT_CI_TEST (SortSuite_HeapThird);
    CUNIT_CI_TEST (SortSuite_HeapFourth);
    CUNIT_CI_TEST (SortSuite_HeapFifth);
    CUNIT_CI_TEST (SortSuite_HeapSixth);

    CUNIT_CI_TEST (SortSuite_MergeFirst);
    CUNIT_CI_TEST (SortSuite_MergeSecond);
    CUNIT_CI_TEST (SortSuite_MergeThird);
    CUNIT_CI_TEST (SortSuite_MergeFourth);
    CUNIT_CI_TEST (SortSuite_MergeFifth);
    CUNIT_CI_TEST (SortSuite_MergeSixth);

    CUNIT_CI_TEST (SortSuite_InplaceMergeFirst);
    CUNIT_CI_TEST (SortSuite_InplaceMergeSecond);
    CUNIT_CI_TEST (SortSuite_InplaceMergeThird);
    CUNIT_CI_TEST (SortSuite_InplaceMergeFourth);
    CUNIT_CI_TEST (SortSuite_InplaceMergeFifth);
    CUNIT_CI_TEST (SortSuite_InplaceMergeSixth);

    CUNIT_CI_TEST (SortSuite_QuickFirst);
    CUNIT_CI_TEST (SortSuite_QuickSecond);
    CUNIT_CI_TEST (SortSuite_QuickThird);
    CUNIT_CI_TEST (SortSuite_QuickFourth);
    CUNIT_CI_TEST (SortSuite_QuickFifth);
    CUNIT_CI_TEST (SortSuite_QuickSixth);
}
