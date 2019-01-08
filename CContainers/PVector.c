#include "PVector.h"
#include "Utils.h"
#include "Errors.h"

#include <ShortTypes.h>
#include <stdlib.h>

typedef struct
{
    uint capacity;
    uint size;
    void **buffer;
} PVector;

PVectorHandle PVector_Create (uint initialCapacity)
{
    PVector *vector = malloc (sizeof (PVector));
    vector->capacity = initialCapacity;
    vector->size = 0;
    vector->buffer = malloc (sizeof (void *) * initialCapacity);
    return (PVectorHandle) vector;
}

void PVector_Destruct (PVectorHandle handle, void (*DestructCallback) (void **item))
{
    PVector *vector = (PVector *) handle;
    PVectorIterator_ForEach (PVector_Begin (handle), PVector_End (handle), DestructCallback);
    free (vector->buffer);
    free (vector);
}

uint PVector_Size (PVectorHandle handle)
{
    PVector *vector = (PVector *) handle;
    return vector->size;
}

PVectorIterator PVector_Begin (PVectorHandle handle)
{
    PVector *vector = (PVector *) handle;
    return (PVectorIterator) vector->buffer;
}

PVectorIterator PVector_End (PVectorHandle handle)
{
    PVector *vector = (PVector *) handle;
    return (PVectorIterator) (vector->buffer + vector->size);
}

PVectorIterator PVector_At (PVectorHandle handle, uint index)
{
    PVector *vector = (PVector *) handle;
    if (index >= vector->size)
    {
        CContainers_SetLastError (PVECTOR_ERROR_OUT_OF_BOUNDS);
        return NULL;
    }

    return (PVectorIterator) (vector->buffer + index);
}

PVectorIterator PVector_Insert (PVectorHandle handle, PVectorIterator where, void *value)
{
    PVector *vector = (PVector *) handle;
    if (where < PVector_Begin (handle) || where > PVector_End (handle))
    {
        CContainers_SetLastError (PVECTOR_ERROR_INCORRECT_ITERATOR);
        return NULL;
    }

    vector->size += 1;
    if (vector->size > vector->capacity)
    {
        ulint index = where - PVector_Begin (handle);
        int increase = vector->capacity / 5;
        vector->capacity += increase > 0 ? increase : 1;
        vector->buffer = realloc (vector->buffer, sizeof (void **) * vector->capacity);
        where = PVector_Begin (handle) + index;
    }

    PVectorIterator_ForEachReversed (PVectorIterator_Next (where),
            PVectorIterator_Previous (PVector_End (handle)), PVectorCallback_MoveRight);
    
    *PVectorIterator_ValueAt (where) = value;
    return where;
}

PVectorIterator PVector_Erase (PVectorHandle handle, PVectorIterator iterator)
{
    PVector *vector = (PVector *) handle;
    if (iterator < PVector_Begin (handle) || iterator >= PVector_End (handle))
    {
        CContainers_SetLastError (PVECTOR_ERROR_INCORRECT_ITERATOR);
        return NULL;
    }

    vector->size -= 1;
    PVectorIterator_ForEach (iterator, PVector_End (handle), PVectorCallback_MoveLeft);

    if (iterator >= PVector_End (handle))
    {
        return PVector_End (handle);
    }
    else
    {
        return iterator;
    }
}

PVectorIterator PVectorIterator_Next (PVectorIterator iterator)
{
    return ++iterator;
}

PVectorIterator PVectorIterator_Previous (PVectorIterator iterator)
{
    return --iterator;
}

void **PVectorIterator_ValueAt (PVectorIterator iterator)
{
    return iterator;
}

void PVectorIterator_ForEach (PVectorIterator begin, PVectorIterator end, void (*Callback) (void **item))
{
    while (begin != end)
    {
        Callback (PVectorIterator_ValueAt (begin));
        begin = PVectorIterator_Next (begin);
    }
}

void PVectorIterator_ForEachReversed (PVectorIterator begin, PVectorIterator last, void (*Callback) (void **item))
{
    while (last >= begin)
    {
        Callback (PVectorIterator_ValueAt (last));
        last = PVectorIterator_Previous (last);
    }
}

void PVectorCallback_MoveLeft (void **item)
{
    *item = *(item + 1);
}

void PVectorCallback_MoveRight (void **item)
{
    *item = *(item - 1);
}
