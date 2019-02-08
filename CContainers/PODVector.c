#include "PODVector.h"
#include "Utils.h"
#include "Errors.h"
#include <stdlib.h>
#include <memory.h>

typedef struct
{
    ulint elementSize;
    ulint size;
    ulint capacity;
    char *buffer;
} PODVector;

PODVectorHandle PODVector_Create (ulint initialCapacity, ulint elementSize)
{
    PODVector *vector = malloc (sizeof (PODVector));
    vector->elementSize = elementSize;
    vector->size = 0;
    vector->capacity = initialCapacity;
    vector->buffer = malloc (initialCapacity * elementSize);
    return (PODVectorHandle) vector;
}

void PODVector_Destruct (PODVectorHandle handle, void (*DestructCallback) (char *itemStart))
{
    PODVector *vector = (PODVector *) handle;
    PODVector_ForEach (handle, 0, vector->size, DestructCallback);

    free (vector->buffer);
    free (vector);
}

ulint PODVector_Size (PODVectorHandle handle)
{
    PODVector *vector = (PODVector *) handle;
    return vector->size;
}

ulint PODVector_Capacity (PODVectorHandle handle)
{
    PODVector *vector = (PODVector *) handle;
    return vector->capacity;
}

void PODVector_Realloc (PODVectorHandle handle, ulint newCapacity)
{
    PODVector *vector = (PODVector *) handle;
    vector->buffer = realloc (vector->buffer, newCapacity * vector->elementSize);
    vector->capacity = newCapacity;

    if (newCapacity < vector->size)
    {
        vector->size = newCapacity;
    }
}

char *PODVector_Begin (PODVectorHandle handle)
{
    PODVector *vector = (PODVector *) handle;
    return vector->buffer;
}

char *PODVector_End (PODVectorHandle handle)
{
    PODVector *vector = (PODVector *) handle;
    return vector->buffer + vector->size * vector->elementSize;
}

char *PODVector_At (PODVectorHandle handle, ulint index)
{
    PODVector *vector = (PODVector *) handle;
    if (index >= vector->size)
    {
        CContainers_SetLastError (PODVECTOR_ERROR_OUT_OF_BOUNDS);
        return NULL;
    }

    return vector->buffer + index * vector->elementSize;
}

char PODVector_Insert (PODVectorHandle handle, ulint where, char *value)
{
    PODVector *vector = (PODVector *) handle;
    if (where > vector->size)
    {
        CContainers_SetLastError (PODVECTOR_ERROR_INCORRECT_POSITION);
        return 0;
    }

    vector->size += 1;
    ulint elementSize = vector->elementSize;
    ulint size = vector->size;

    if (size > vector->capacity)
    {
        vector->capacity *= 2;
        vector->buffer = realloc (vector->buffer, vector->capacity * elementSize);
    }

    if (where < size - 1)
    {
        char *position = vector->buffer + (size - 1) * elementSize;
        char *insertionPoint = vector->buffer + where * elementSize;

        while (position >= insertionPoint)
        {
            memcpy (position, position - elementSize, elementSize);
            position -= elementSize;
        }
    }

    memcpy (vector->buffer + where * vector->elementSize, value, vector->elementSize);
    return 1;
}

char PODVector_Erase (PODVectorHandle handle, ulint position)
{
    PODVector *vector = (PODVector *) handle;
    if (position >= vector->size)
    {
        CContainers_SetLastError (PODVECTOR_ERROR_INCORRECT_POSITION);
        return 0;
    }

    vector->size -= 1;
    ulint elementSize = vector->elementSize;

    if (position != vector->size)
    {
        char *current = vector->buffer + position * elementSize;
        char *end = vector->buffer + vector->size * elementSize;

        while (current != end)
        {
            memcpy (current, current + elementSize, elementSize);
            current += elementSize;
        }
    }

    return 1;
}

void PODVector_ForEach (PODVectorHandle handle, ulint beginIndex, ulint endIndex, void (*Callback) (char *itemStart))
{
    PODVector *vector = (PODVector *) handle;
    char *current = PODVector_At (handle, beginIndex);
    char *end = PODVector_At (handle, endIndex);

    if (end == NULL)
    {
        end = PODVector_End (handle);
    }

    while (current != end)
    {
        Callback (current);
        current += vector->elementSize;
    }
}

void PODVector_ForEachReversed (PODVectorHandle handle, ulint beginIndex, ulint lastIndex,
        void (*Callback) (char *itemStart))
{
    PODVector *vector = (PODVector *) handle;
    char *current = PODVector_At (handle, lastIndex);
    char *begin = PODVector_At (handle, beginIndex);

    while (current != begin)
    {
        Callback (current);
        current -= vector->elementSize;
    }

    Callback (current);
}
