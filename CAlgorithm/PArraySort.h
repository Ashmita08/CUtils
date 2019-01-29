#ifndef __CUTILS_CALGORITHM_PARRAYSORT_H__
#define __CUTILS_CALGORITHM_PARRAYSORT_H__

#include <CContainers/Interfaces.h>

void PArrayHeapSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second));
void PArrayMergeSortedParts (void **begin, void **middle, void **end,
        lint (*Comparator) (const void *first, const void *second));

void PArrayInplaceMergeSortedParts (void **begin, void **middle, void **end,
        lint (*Comparator) (const void *first, const void *second));

void PArrayMergeSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second));
void PArrayInplaceMergeSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second));
void PArrayQuickSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second));

#endif // __CUTILS_CALGORITHM_PARRAYSORT_H__
