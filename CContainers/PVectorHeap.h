#ifndef __CUTILS_CCONTAINERS_PVECTORHEAP_H__
#define __CUTILS_CCONTAINERS_PVECTORHEAP_H__

#include <ShortTypes.h>
#include "Interfaces.h"

typedef void *PVectorHeapHandle;
PVectorHeapHandle PVectorHeap_Create (ulint initialCapacity, lint (*Comparator) (const void *first, const void *second));
/// Creates heap from given PVector. Given PVector would be captured and used as storage by created heap.
PVectorHeapHandle PVectorHeap_Heapify (VirtualHandle vector, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second), char reverseComparator);

void PVectorHeap_Destruct (PVectorHeapHandle handle,
        void (*BaseDestructor) (VirtualHandle vector, void (*DestructCallback) (void **item)),
        void (*DestructCallback) (void **item));

ulint PVectorHeap_Size (PVectorHeapHandle handle);
const void *PVectorHeap_Top (PVectorHeapHandle handle);
void PVectorHeap_Pop (PVectorHeapHandle handle);
void PVectorHeap_Push (PVectorHeapHandle handle, void *value);
void PVectorHeap_ReverseComparator (PVectorHeapHandle handle);

ISizedContainer *PVectorHeap_AsISizedContainer ();
IOrganizerContainer *PVectorHeap_AsIOrganizerContainer ();

#endif // __CUTILS_CCONTAINERS_PVECTORHEAP_H__
