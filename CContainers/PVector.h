#ifndef __CUTILS_CCONTAINERS_PVECTOR_H__
#define __CUTILS_CCONTAINERS_PVECTOR_H__

#include <ShortTypes.h>
#include "Interfaces.h"

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
PVectorIterator PVectorIterator_Jump (PVectorIterator iterator, ulint distance);
PVectorIterator PVectorIterator_Previous (PVectorIterator iterator);
PVectorIterator PVectorIterator_JumpBack (PVectorIterator iterator, ulint distance);
void **PVectorIterator_ValueAt (PVectorIterator iterator);

IOneDirectionIterator *PVectorIterator_AsIOneDirectionIterator ();
IBiDirectionalIterator *PVectorIterator_AsIBiDirectionalIterator ();
ISizedContainer *PVector_AsISizedContainer ();
IIterableContainer *PVector_AsIIterableContainer ();
IMutableContainer *PVector_AsIMutableContainer ();

#endif // __CUTILS_CCONTAINERS_PVECTOR_H__
