#ifndef __CUTILS_CALGORITHM_SORT_H__
#define __CUTILS_CALGORITHM_SORT_H__

#include <CContainers/Interfaces.h>

void HeapSort (VirtualHandle container, ISizedContainer *ISized,
        IIterableContainer *IIterable, IOneDirectionIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second));

#endif // __CUTILS_CALGORITHM_SORT_H__
