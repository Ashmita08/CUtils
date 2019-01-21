#include "PHashMap_vs_stdunorderedmap.h"
#include <ctime>
#include <unordered_map>
#include <cstdio>

extern "C"
{
#include <ShortTypes.h>
#include <CContainers/PHashMap.h>
#include <CContainers/Utils.h>
}

#define TEST_AMOUNT 2000000
#define PHASHMAP_MAXBUCKETLOAD 10
#define PHASHMAP_DEFAULT_BUCKET_COUNT 100

uint HashKey (void *key)
{
    return (uint) key;
}

int KeyCompare (void *first, void *second)
{
    return (int) first - (int) second;
}

void EmptyDestruct (void **item)
{

}

clock_t PHashMap_InsertAndCheckContains (bool reserve)
{
    clock_t begin = clock ();
    PHashMapHandle hashMap = PHashMap_Create (reserve ? TEST_AMOUNT : PHASHMAP_DEFAULT_BUCKET_COUNT,
            PHASHMAP_MAXBUCKETLOAD, HashKey, KeyCompare);

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        PHashMap_Insert (hashMap, (void *) index, nullptr);
    }

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        PHashMap_ContainsKey (hashMap, (void *) index);
    }

    PHashMap_Destruct (hashMap, EmptyDestruct, EmptyDestruct);
    return clock () - begin;
}

clock_t stdunorderedmap_InsertAndCheckContains ()
{
    clock_t begin = clock ();
    auto *map = new std::unordered_map <int, void *> ();

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        map->insert ({index, nullptr});
    }

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        map->count (index);
    }

    delete map;
    return clock () - begin;
}

void PHashMap_vs_stdunorderedmap ()
{
    printf ("PHashMap: items to insert and check -- %d.\n", TEST_AMOUNT);
    printf ("PHashMap, no reserve: %dms.\n", (int) PHashMap_InsertAndCheckContains (false) * 1000 / CLOCKS_PER_SEC);
    printf ("PHashMap, reserve: %dms.\n", (int) PHashMap_InsertAndCheckContains (true) * 1000 / CLOCKS_PER_SEC);
    printf ("std::unordered_set: %dms.\n", (int) stdunorderedmap_InsertAndCheckContains () * 1000 / CLOCKS_PER_SEC);
}