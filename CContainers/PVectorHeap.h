#ifndef __CUTILS_CCONTAINERS_PVECTORHEAP_H__
#define __CUTILS_CCONTAINERS_PVECTORHEAP_H__

#include <ShortTypes.h>
#include "PVector.h"
#include "Interfaces.h"

typedef void *PVectorHeapHandle;
PVectorHeapHandle PVectorHeap_Create (uint initialCapacity, int (*Comparator) (const void *first, const void *second));
/// Creates heap from given PVector. Given PVector would be captured and used as storage by created heap.
PVectorHeapHandle PVectorHeap_Heapify (PVectorHandle vectorHandle,
        int (*Comparator) (const void *first, const void *second));
void PVectorHeap_Destruct (PVectorHeapHandle handle, void (*DestructCallback) (void **item));

ulint PVectorHeap_Size (PVectorHeapHandle handle);
const void *PVectorHeap_Top (PVectorHeapHandle handle);
void PVectorHeap_Pop (PVectorHeapHandle handle);
void PVectorHeap_Push (PVectorHeapHandle handle, void *value);

ISizedContainer *PVectorHeap_AsISizedContainer ();
IOrganizerContainer *PVectorHeap_AsIOrganizerContainer ();

#endif // __CUTILS_CCONTAINERS_PVECTORHEAP_H__
