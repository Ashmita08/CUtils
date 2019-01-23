#ifndef __CUTILS_CCONTAINERS_PHASHMAP_H__
#define __CUTILS_CCONTAINERS_PHASHMAP_H__

#include <ShortTypes.h>
#include "Interfaces.h"

typedef void *PHashMapHandle;
PHashMapHandle PHashMap_Create (ulint initialBucketCount, ulint maxBucketLoad,
        ulint (*HashKey) (void *key), lint (*KeyCompare) (void *first, void *second));
void PHashMap_Destruct (PHashMapHandle handle, void (*KeyDestructCallback) (void **key),
        void (*ValueDestructCallback) (void **value));

ulint PHashMap_Size (PHashMapHandle handle);
char PHashMap_ContainsKey (PHashMapHandle handle, void *key);
char PHashMap_Insert (PHashMapHandle handle, void *key, void *value);
void **PHashMap_GetValue (PHashMapHandle handle, void *key);
char PHashMap_Erase (PHashMapHandle handle, void *key, void (*KeyDestructCallback) (void **key),
        void (*ValueDestructCallback) (void **value));

ISizedContainer *PHashMap_AsISizedContainer ();
IMapContainer *PHashMap_AsIMapContainer ();

#endif // __CUTILS_CCONTAINERS_PHASHMAP_H__
