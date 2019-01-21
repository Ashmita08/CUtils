#ifndef __CUTILS_CCONTAINERS_PHASHMAP_H__
#define __CUTILS_CCONTAINERS_PHASHMAP_H__

#include <ShortTypes.h>

typedef void *PHashMapHandle;
PHashMapHandle PHashMap_Create (uint initialBucketCount, uint maxBucketLoad,
        uint (*HashKey) (void *key), int (*KeyCompare) (void *first, void *second));
void PHashMap_Destroy (PHashMapHandle handle, void (*KeyDestructCallback) (void **key),
        void (*ValueDestructCallback) (void **value));

uint PHashMap_Size (PHashMapHandle handle);
char PHashMap_ContainsKey (PHashMapHandle handle, void *key);
char PHashMap_Insert (PHashMapHandle handle, void *key, void *value);
void **PHashMap_GetValue (PHashMapHandle handle, void *key);
char PHashMap_Erase (PHashMapHandle handle, void *key, void (*KeyDestructCallback) (void **key),
        void (*ValueDestructCallback) (void **value));

#endif // __CUTILS_CCONTAINERS_PHASHMAP_H__
