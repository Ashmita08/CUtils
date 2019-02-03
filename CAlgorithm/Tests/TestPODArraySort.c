#include "TestPODArraySort.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CAlgorithm/PODArraySort.h>
#include <CContainers/PVector.h>
#include <CContainers/Utils.h>

#define FIRST_TEST_SIZE 7
static int FirstTest_Input [] = {1, 2, 3, 4, 5, 6, 7};
static int FirstTest_Output [] = {1, 2, 3, 4, 5, 6, 7};

#define SECOND_TEST_SIZE 7
static int SecondTest_Input [] = {7, 6, 5, 4, 3, 2, 1};
static int SecondTest_Output [] = {1, 2, 3, 4, 5, 6, 7};

#define THIRD_TEST_SIZE 8
static int ThirdTest_Input [] = {34, 12, 31, 66, 98, 45, 23, 11};
static int ThirdTest_Output [] = {11, 12, 23, 31, 34, 45, 66, 98};

#define FOURTH_TEST_SIZE 10
static int FourthTest_Input [] = {1, 1, 1, 2, 3, 4, 4, 5, 6, 7};
static int FourthTest_Output [] = {1, 1, 1, 2, 3, 4, 4, 5, 6, 7};

#define FIFTH_TEST_SIZE 10
static int FifthTest_Input [] = {7, 7, 7, 6, 5, 5, 4, 3, 2, 1};
static int FifthTest_Output [] = {1, 2, 3, 4, 5, 5, 6, 7, 7, 7};

#define SIXTH_TEST_SIZE 11
static int SixthTest_Input [] = {34, 12, 34, 31, 66, 11, 45, 98, 45, 23, 11};
static int SixthTest_Output [] = {11, 11, 12, 23, 31, 34, 34, 45, 45, 66, 98};

static lint Comparator (const char *first, const char *second)
{
    return (lint) (**(const int **) second - **(const int **) first);
}

static void PODArraySortSuite_Helper_FillVector (void **vector, uint size, int *input)
{
    while (size--)
    {
        *vector = input;
        ++vector;
        ++input;
    }
}

void PODArraySortSuite_Helper_CheckVector (void **vector, uint size, const int *expectedOutput)
{
    int index = 0;
    while (size--)
    {
        int actual = **(int **) vector;
        int expected = *expectedOutput;

        if (actual != expected)
        {
            printf ("\n    Error at index %d (e/a): %d, %d.", index, expected, actual);
            CU_FAIL ("Incorrect order found!");
        }

        ++index;
        ++vector;
        ++expectedOutput;
    }
}

static void PODArraySortSuite_Helper_MergeSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PODArraySortSuite_Helper_FillVector (vector, size, input);
    PODArrayMergeSort ((char *) vector, (char *) (vector + size), sizeof (int *), Comparator);

    PODArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PODArraySortSuite_Helper_InplaceMergeSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PODArraySortSuite_Helper_FillVector (vector, size, input);
    PODArrayInplaceMergeSort ((char *) vector, (char *) (vector + size), sizeof (int *), Comparator);

    PODArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PODArraySortSuite_Helper_HeapSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PODArraySortSuite_Helper_FillVector (vector, size, input);
    PODArrayHeapSort ((char *) vector, (char *) (vector + size), sizeof (int *), Comparator);

    PODArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PODArraySortSuite_Helper_QuickSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PODArraySortSuite_Helper_FillVector (vector, size, input);
    PODArrayQuickSort ((char *) vector, (char *) (vector + size), sizeof (int *), Comparator);

    PODArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PODArraySortSuite_Helper_IntroSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PODArraySortSuite_Helper_FillVector (vector, size, input);
    PODArrayIntroSort ((char *) vector, (char *) (vector + size), sizeof (int *), Comparator);

    PODArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PODArraySortSuite_Setup ()
{

}

static void PODArraySortSuite_Teardown ()
{

}

static void PODArraySortSuite_MergeFirst ()
{
    PODArraySortSuite_Helper_MergeSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PODArraySortSuite_MergeSecond ()
{
    PODArraySortSuite_Helper_MergeSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PODArraySortSuite_MergeThird ()
{
    PODArraySortSuite_Helper_MergeSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PODArraySortSuite_MergeFourth ()
{
    PODArraySortSuite_Helper_MergeSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PODArraySortSuite_MergeFifth ()
{
    PODArraySortSuite_Helper_MergeSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PODArraySortSuite_MergeSixth ()
{
    PODArraySortSuite_Helper_MergeSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void PODArraySortSuite_InplaceMergeFirst ()
{
    PODArraySortSuite_Helper_InplaceMergeSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PODArraySortSuite_InplaceMergeSecond ()
{
    PODArraySortSuite_Helper_InplaceMergeSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PODArraySortSuite_InplaceMergeThird ()
{
    PODArraySortSuite_Helper_InplaceMergeSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PODArraySortSuite_InplaceMergeFourth ()
{
    PODArraySortSuite_Helper_InplaceMergeSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PODArraySortSuite_InplaceMergeFifth ()
{
    PODArraySortSuite_Helper_InplaceMergeSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PODArraySortSuite_InplaceMergeSixth ()
{
    PODArraySortSuite_Helper_InplaceMergeSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void PODArraySortSuite_HeapFirst ()
{
    PODArraySortSuite_Helper_HeapSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PODArraySortSuite_HeapSecond ()
{
    PODArraySortSuite_Helper_HeapSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PODArraySortSuite_HeapThird ()
{
    PODArraySortSuite_Helper_HeapSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PODArraySortSuite_HeapFourth ()
{
    PODArraySortSuite_Helper_HeapSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PODArraySortSuite_HeapFifth ()
{
    PODArraySortSuite_Helper_HeapSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PODArraySortSuite_HeapSixth ()
{
    PODArraySortSuite_Helper_HeapSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void PODArraySortSuite_QuickFirst ()
{
    PODArraySortSuite_Helper_QuickSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PODArraySortSuite_QuickSecond ()
{
    PODArraySortSuite_Helper_QuickSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PODArraySortSuite_QuickThird ()
{
    PODArraySortSuite_Helper_QuickSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PODArraySortSuite_QuickFourth ()
{
    PODArraySortSuite_Helper_QuickSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PODArraySortSuite_QuickFifth ()
{
    PODArraySortSuite_Helper_QuickSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PODArraySortSuite_QuickSixth ()
{
    PODArraySortSuite_Helper_QuickSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void PODArraySortSuite_IntroFirst ()
{
    PODArraySortSuite_Helper_IntroSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PODArraySortSuite_IntroSecond ()
{
    PODArraySortSuite_Helper_IntroSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PODArraySortSuite_IntroThird ()
{
    PODArraySortSuite_Helper_IntroSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PODArraySortSuite_IntroFourth ()
{
    PODArraySortSuite_Helper_IntroSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PODArraySortSuite_IntroFifth ()
{
    PODArraySortSuite_Helper_IntroSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PODArraySortSuite_IntroSixth ()
{
    PODArraySortSuite_Helper_IntroSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

void RegisterPODArraySortSuite ()
{
    CU_CI_DEFINE_SUITE ("Sort", NULL, NULL, PODArraySortSuite_Setup, PODArraySortSuite_Teardown);
    CUNIT_CI_TEST (PODArraySortSuite_MergeFirst);
    CUNIT_CI_TEST (PODArraySortSuite_MergeSecond);
    CUNIT_CI_TEST (PODArraySortSuite_MergeThird);
    CUNIT_CI_TEST (PODArraySortSuite_MergeFourth);
    CUNIT_CI_TEST (PODArraySortSuite_MergeFifth);
    CUNIT_CI_TEST (PODArraySortSuite_MergeSixth);

    CUNIT_CI_TEST (PODArraySortSuite_InplaceMergeFirst);
    CUNIT_CI_TEST (PODArraySortSuite_InplaceMergeSecond);
    CUNIT_CI_TEST (PODArraySortSuite_InplaceMergeThird);
    CUNIT_CI_TEST (PODArraySortSuite_InplaceMergeFourth);
    CUNIT_CI_TEST (PODArraySortSuite_InplaceMergeFifth);
    CUNIT_CI_TEST (PODArraySortSuite_InplaceMergeSixth);

    CUNIT_CI_TEST (PODArraySortSuite_HeapFirst);
    CUNIT_CI_TEST (PODArraySortSuite_HeapSecond);
    CUNIT_CI_TEST (PODArraySortSuite_HeapThird);
    CUNIT_CI_TEST (PODArraySortSuite_HeapFourth);
    CUNIT_CI_TEST (PODArraySortSuite_HeapFifth);
    CUNIT_CI_TEST (PODArraySortSuite_HeapSixth);

    CUNIT_CI_TEST (PODArraySortSuite_QuickFirst);
    CUNIT_CI_TEST (PODArraySortSuite_QuickSecond);
    CUNIT_CI_TEST (PODArraySortSuite_QuickThird);
    CUNIT_CI_TEST (PODArraySortSuite_QuickFourth);
    CUNIT_CI_TEST (PODArraySortSuite_QuickFifth);
    CUNIT_CI_TEST (PODArraySortSuite_QuickSixth);

    CUNIT_CI_TEST (PODArraySortSuite_IntroFirst);
    CUNIT_CI_TEST (PODArraySortSuite_IntroSecond);
    CUNIT_CI_TEST (PODArraySortSuite_IntroThird);
    CUNIT_CI_TEST (PODArraySortSuite_IntroFourth);
    CUNIT_CI_TEST (PODArraySortSuite_IntroFifth);
    CUNIT_CI_TEST (PODArraySortSuite_IntroSixth);
}
