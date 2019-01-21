#include "PHashMap.h"
#include "Utils.h"
#include "Errors.h"
#include "PVector.h"
#include <stdlib.h>

#define BUCKET_COUNT_INCREASE_FACTOR 1.5

typedef struct
{
    void *key;
    void *value;
} KeyValuePair;

typedef struct
{
    uint size;
    uint maxLoad;
    /// Vector of vector handles, each inner vector is key-value pair.
    PVectorHandle buckets;

    uint (*HashFunction) (void *key);
    int (*KeyComparator) (void *first, void *second);
} PHashMap;

static void EmptyDeleteCallback (void **item)
{

}

static void PHashMap_BucketDestroyCallbackKeepPairs (void **bucket)
{
    PVector_Destruct (*bucket, EmptyDeleteCallback);
}

static void PHashMap_BucketDestroyCallback (void **bucket)
{
    PVector_Destruct (*bucket, ContainerCallback_Free);
}

static void PHashMap_InitBuckets (PHashMap *hashMap, uint bucketsCount)
{
    PVectorHandle buckets = PVector_Create (bucketsCount);
    while (bucketsCount--)
    {
        PVectorHandle bucket = PVector_Create (hashMap->maxLoad);
        PVector_Insert (buckets, PVector_End (buckets), bucket);
    }

    hashMap->buckets = buckets;
}

static PVectorHandle PHashMap_GetBucket (PHashMap *hashMap, void *key)
{
    uint bucketIndex = hashMap->HashFunction (key) % PVector_Size (hashMap->buckets);
    return *PVectorIterator_ValueAt (PVector_At (hashMap->buckets, bucketIndex));
}

static KeyValuePair *PHashMap_GetPairInBucket (PHashMap *hashMap, PVectorHandle bucket, void *key)
{
    PVectorIterator keyValueIterator = PVector_Begin (bucket);
    while (keyValueIterator != PVector_End (bucket))
    {
        KeyValuePair *pair = *PVectorIterator_ValueAt (keyValueIterator);
        if (hashMap->KeyComparator (key, pair->key) == 0)
        {
            return pair;
        }

        keyValueIterator = PVectorIterator_Next (keyValueIterator);
    }

    return NULL;
}

static KeyValuePair *PHashMap_GetPair (PHashMap *hashMap, void *key)
{
    return PHashMap_GetPairInBucket (hashMap, PHashMap_GetBucket (hashMap, key), key);
}

static void PHashMap_Rehash (PHashMap *hashMap)
{
    PVectorHandle oldBuckets = hashMap->buckets;
    PHashMap_InitBuckets (hashMap, (uint) (PVector_Size (oldBuckets) * BUCKET_COUNT_INCREASE_FACTOR));
    PVectorIterator bucketIterator = PVector_Begin (oldBuckets);

    while (bucketIterator != PVector_End (oldBuckets))
    {
        PVectorHandle bucket = *PVectorIterator_ValueAt (bucketIterator);
        PVectorIterator keyValueIterator = PVector_Begin (bucket);

        while (keyValueIterator != PVector_End (bucket))
        {
            KeyValuePair *pair = *PVectorIterator_ValueAt (keyValueIterator);
            PVectorHandle newBucket = PHashMap_GetBucket (hashMap, pair->key);
            PVector_Insert (newBucket, PVector_End (newBucket), pair);
            keyValueIterator = PVectorIterator_Next (keyValueIterator);
        }

        bucketIterator = PVectorIterator_Next (bucketIterator);
    }

    PVector_Destruct (oldBuckets, PHashMap_BucketDestroyCallbackKeepPairs);
}

PHashMapHandle PHashMap_Create (uint initialBucketCount, uint maxBucketLoad,
        uint (*HashKey) (void *key), int (*KeyCompare) (void *first, void *second))
{
    PHashMap *hashMap = malloc (sizeof (PHashMap));
    hashMap->size = 0;
    hashMap->maxLoad = maxBucketLoad;

    hashMap->HashFunction = HashKey;
    hashMap->KeyComparator = KeyCompare;
    PHashMap_InitBuckets (hashMap, initialBucketCount);
    return (PHashMapHandle) hashMap;
}

void PHashMap_Destroy (PHashMapHandle handle, void (*KeyDestructCallback) (void **key),
        void (*ValueDestructCallback) (void **value))
{
    PHashMap *hashMap = (PHashMap *) handle;
    PVectorIterator bucketIterator = PVector_Begin (hashMap->buckets);

    while (bucketIterator != PVector_End (hashMap->buckets))
    {
        PVectorHandle bucket = *PVectorIterator_ValueAt (bucketIterator);
        PVectorIterator keyValueIterator = PVector_Begin (bucket);

        while (keyValueIterator != PVector_End (bucket))
        {
            KeyValuePair *pair = *PVectorIterator_ValueAt (keyValueIterator);
            KeyDestructCallback (&(pair->key));
            ValueDestructCallback (&(pair->value));
            keyValueIterator = PVectorIterator_Next (keyValueIterator);
        }

        bucketIterator = PVectorIterator_Next (bucketIterator);
    }

    PVector_Destruct (hashMap->buckets, PHashMap_BucketDestroyCallback);
}

uint PHashMap_Size (PHashMapHandle handle)
{
    PHashMap *hashMap = (PHashMap *) handle;
    return hashMap->size;
}

char PHashMap_ContainsKey (PHashMapHandle handle, void *key)
{
    PHashMap *hashMap = (PHashMap *) handle;
    return PHashMap_GetPair (hashMap, key) == NULL;
}

char PHashMap_Insert (PHashMapHandle handle, void *key, void *value)
{
    PHashMap *hashMap = (PHashMap *) handle;
    PVectorHandle bucket = PHashMap_GetBucket (hashMap, key);
    KeyValuePair *pair = PHashMap_GetPairInBucket (hashMap, bucket, key);

    if (pair != NULL)
    {
        return 0;
    }

    pair = malloc (sizeof (KeyValuePair));
    pair->key = key;
    pair->value = value;
    PVector_Insert (bucket, PVector_End (bucket), pair);

    if (PVector_Size (bucket) > hashMap->maxLoad)
    {
        PHashMap_Rehash (hashMap);
    }

    return 1;
}

void **PHashMap_GetValue (PHashMapHandle handle, void *key)
{
    PHashMap *hashMap = (PHashMap *) handle;
    KeyValuePair *pair = PHashMap_GetPair (hashMap, key);
    return pair != NULL ? &(pair->value) : NULL;
}