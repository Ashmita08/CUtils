#ifndef __CUTILS_CCONTAINERS_PVECTORHEAP_H__
#define __CUTILS_CCONTAINERS_PVECTORHEAP_H__

#include <ShortTypes.h>
#include "Interfaces.h"

typedef void *PHeapHandle;
PHeapHandle PHeap_Create (ulint initialCapacity, lint (*Comparator) (const void *first, const void *second));
/// Creates heap from given PVector. Given PVector would be captured and used as storage by created heap.
PHeapHandle PHeap_Heapify (VirtualHandle vector, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second), char reverseComparator);

void PHeap_Destruct (PHeapHandle handle,
        void (*BaseDestructor) (VirtualHandle vector, void (*DestructCallback) (void **item)),
        void (*DestructCallback) (void **item));

ulint PHeap_Size (PHeapHandle handle);
const void *PHeap_Top (PHeapHandle handle);
void PHeap_Pop (PHeapHandle handle);
void PHeap_Push (PHeapHandle handle, void *value);
void PHeap_ReverseComparator (PHeapHandle handle);

ISizedContainer *PHeap_AsISizedContainer ();
IOrganizerContainer *PHeap_AsIOrganizerContainer ();

#endif // __CUTILS_CCONTAINERS_PVECTORHEAP_H__
