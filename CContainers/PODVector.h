#ifndef __CUTILS_CCONTAINERS_PODVECTOR_H__
#define __CUTILS_CCONTAINERS_PODVECTOR_H__

#include <ShortTypes.h>
#include "Interfaces.h"

typedef void *PODVectorHandle;

PODVectorHandle PODVector_Create (ulint initialCapacity, ulint elementSize);
void PODVector_Destruct (PODVectorHandle handle, void (*DestructCallback) (char *itemStart));

ulint PODVector_Size (PODVectorHandle handle);
ulint PODVector_Capacity (PODVectorHandle handle);
void PODVector_Realloc (PODVectorHandle handle, ulint newCapacity);

char *PODVector_Begin (PODVectorHandle handle);
char *PODVector_End (PODVectorHandle handle);
char *PODVector_At (PODVectorHandle handle, ulint index);

char PODVector_Insert (PODVectorHandle handle, ulint where, char *value);
char PODVector_Erase (PODVectorHandle handle, ulint position);

void PODVector_ForEach (PODVectorHandle handle, ulint beginIndex, ulint endIndex, void (*Callback) (char *itemStart));
void PODVector_ForEachReversed (PODVectorHandle handle, ulint beginIndex, ulint lastIndex,
        void (*Callback) (char *itemStart));

#endif // __CUTILS_CCONTAINERS_PODVECTOR_H__
