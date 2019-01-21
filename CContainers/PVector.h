#ifndef __CUTILS_CCONTAINERS_PVECTOR_H__
#define __CUTILS_CCONTAINERS_PVECTOR_H__

#include <ShortTypes.h>

typedef void *PVectorHandle;
typedef void **PVectorIterator;

PVectorHandle PVector_Create (ulint initialCapacity);
void PVector_Destruct (PVectorHandle handle, void (*DestructCallback) (void **item));
ulint PVector_Size (PVectorHandle handle);

PVectorIterator PVector_Begin (PVectorHandle handle);
PVectorIterator PVector_End (PVectorHandle handle);
PVectorIterator PVector_At (PVectorHandle handle, ulint index);

PVectorIterator PVector_Insert (PVectorHandle handle, PVectorIterator where, void *value);
PVectorIterator PVector_Erase (PVectorHandle handle, PVectorIterator iterator);

PVectorIterator PVectorIterator_Next (PVectorIterator iterator);
PVectorIterator PVectorIterator_Previous (PVectorIterator iterator);
void **PVectorIterator_ValueAt (PVectorIterator iterator);

void PVectorIterator_ForEach (PVectorIterator begin, PVectorIterator end, void (*Callback) (void **item));
void PVectorIterator_ForEachReversed (PVectorIterator begin, PVectorIterator last, void (*Callback) (void **item));

void PVectorCallback_MoveLeft (void **item);
void PVectorCallback_MoveRight (void **item);

#endif // __CUTILS_CCONTAINERS_PVECTOR_H__
