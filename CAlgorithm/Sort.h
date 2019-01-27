#ifndef __CUTILS_CALGORITHM_SORT_H__
#define __CUTILS_CALGORITHM_SORT_H__

#include <CContainers/Interfaces.h>

void HeapSort (VirtualHandle container, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second));

void MergeSortedParts (VirtualHandle begin, VirtualHandle middle, VirtualHandle end,
        ulint leftPartSize, ulint rightPartSize, IOneDirectionIterator *IIterator, 
        lint (*Comparator) (const void *first, const void *second));

void MergeSort (VirtualHandle begin, VirtualHandle end, ulint size, IOneDirectionIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second));

#endif // __CUTILS_CALGORITHM_SORT_H__
