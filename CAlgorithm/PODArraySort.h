#ifndef __CUTILS_CALGORITHM_PODARRAYSORT_H__
#define __CUTILS_CALGORITHM_PODARRAYSORT_H__

#include <CContainers/Interfaces.h>

void PODArrayHeapSort (char *begin, const char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second));

void PODArrayMergeSortedParts (char *begin, char *middle, char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second));

void PODArrayInplaceMergeSortedParts (char *begin, char *middle, char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second));

void PODArrayMergeSort (char *begin, const char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second));

void PODArrayInplaceMergeSort(char *begin, const char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second));

void PODArrayQuickSort (char *begin, char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second));

void PODArrayIntroSort (char *begin, char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second));

#endif // __CUTILS_CALGORITHM_PODARRAYSORT_H__
