#ifndef __CUTILS_CALGORITHM_SORT_H__
#define __CUTILS_CALGORITHM_SORT_H__

#include <CContainers/Interfaces.h>

typedef void (*SortInterface) (VirtualHandle container, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second));

void HeapSort (VirtualHandle container, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second));

#endif // __CUTILS_CALGORITHM_SORT_H__
