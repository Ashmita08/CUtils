#include "PODMemory.h"
#include <stdint.h>

void PODMemory_Swap (char *firstElement, char *secondElement, ulint elementSize)
{
#define SwapCore(baseType) \
    while (elementSize >= sizeof (baseType)) \
    { \
        baseType buffer = *(baseType *) firstElement; \
        *(baseType *) firstElement = *(baseType *) secondElement; \
        *(baseType *) secondElement = buffer; \
        \
        firstElement += sizeof (baseType); \
        secondElement += sizeof (baseType); \
        elementSize -= sizeof (baseType); \
    } \
    \
    if (!elementSize) return;

    SwapCore (llint);
    SwapCore (lint);
    SwapCore (int32_t);
    SwapCore (int16_t);
    SwapCore (char);
}

void PODMemory_Copy (char *destination, const char *source, ulint elementSize)
{
#define CopyCore(baseType) \
    while (elementSize >= sizeof (baseType)) \
    { \
        *(baseType *) destination = *(baseType *) source; \
        destination += sizeof (baseType); \
        source += sizeof (baseType); \
        elementSize -= sizeof (baseType); \
    } \
    \
    if (!elementSize) return;

    CopyCore (llint);
    CopyCore (lint);
    CopyCore (int32_t);
    CopyCore (int16_t);
    CopyCore (char);
}

void PODMemory_MoveRight (char *memory, ulint elementSize, ulint offset)
{
    char *memoryEnd = memory + elementSize;
#define MoveRightCore(baseType) \
    while ((memoryEnd -= sizeof (baseType)) >= memory) \
    { \
        *(baseType *) (memoryEnd + offset) = *(baseType *) (memoryEnd); \
    } \
    \
    memoryEnd += sizeof (baseType); \
    if (memoryEnd == memory) return;

    MoveRightCore (llint);
    MoveRightCore (lint);
    MoveRightCore (int32_t);
    MoveRightCore (int16_t);
    MoveRightCore (char);
}

void PODMemory_MoveLeft (char *memory, ulint elementSize, ulint offset)
{
    char *memoryEnd = memory + elementSize;
#define MoveLeftCore(baseType) \
    while (memory < memoryEnd) \
    { \
        *(baseType *) (memory - offset) = *(baseType *) memory; \
        memory += sizeof (baseType); \
    } \
    \
    if (memory == memoryEnd) return; \
    memory -= sizeof (baseType);

    MoveLeftCore (llint);
    MoveLeftCore (lint);
    MoveLeftCore (int32_t);
    MoveLeftCore (int16_t);
    MoveLeftCore (char);
}
