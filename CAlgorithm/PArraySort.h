#ifndef __CUTILS_CALGORITHM_PARRAYSORT_H__
#define __CUTILS_CALGORITHM_PARRAYSORT_H__

#include <CContainers/Interfaces.h>

void PArrayMergeSortedParts (void **begin, void **middle, void **end,
        lint (*Comparator) (const void *first, const void *second));
void PArrayMergeSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second));

#endif // __CUTILS_CALGORITHM_PARRAYSORT_H__
