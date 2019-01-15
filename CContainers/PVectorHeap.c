#include "PVectorHeap.h"
#include "Utils.h"
#include "Errors.h"
#include <stdlib.h>

typedef struct
{
    PVectorHandle vector;
    int (*Comparator) (const void *first, const void *second);
} PVectorHeap;

static void PVectorHeap_SiftDown (PVectorHeap *heap, uint elementIndex)
{
    uint leftChild = elementIndex * 2 + 1;
    uint rightChild = elementIndex * 2 + 2;
    uint size = PVector_Size (heap->vector);

    if (rightChild > size)
    {
        return;
    }

    void **current = PVectorIterator_ValueAt (PVector_At (heap->vector, elementIndex));
    void **left = PVectorIterator_ValueAt (PVector_At (heap->vector, leftChild));

    if (rightChild == size)
    {
        if (heap->Comparator (*left, *current) > 0)
        {
            void *temp = *current;
            *current = *left;
            *left = temp;
        }
    }
    else
    {
        void **right = PVectorIterator_ValueAt (PVector_At (heap->vector, rightChild));
        void **better = NULL;
        uint betterIndex;

        if (heap->Comparator (*right, *left) > 0)
        {
            better = right;
            betterIndex = rightChild;
        }
        else
        {
            better = left;
            betterIndex = leftChild;
        }

        if (heap->Comparator (*better, *current) > 0)
        {
            void *temp = *current;
            *current = *better;
            *better = temp;
            PVectorHeap_SiftDown (heap, betterIndex);
        }
    }
}

static void PVectorHeap_SiftUp (PVectorHeap *heap, uint elementIndex)
{
    if (elementIndex == 0)
    {
        return;
    }

    uint parentIndex = elementIndex / 2;
    if (elementIndex % 2 == 0)
    {
        --parentIndex;
    }

    void **current = PVectorIterator_ValueAt (PVector_At (heap->vector, elementIndex));
    void **parent = PVectorIterator_ValueAt (PVector_At (heap->vector, parentIndex));

    if (heap->Comparator (*current, *parent) > 0)
    {
        void *temp = *current;
        *current = *parent;
        *parent = temp;
        PVectorHeap_SiftUp (heap, parentIndex);
    }
}

PVectorHeapHandle PVectorHeap_Create (uint initialCapacity, int (*Comparator) (const void *first, const void *second))
{
    PVectorHeap *heap = malloc (sizeof (PVectorHeap));
    heap->vector = PVector_Create (initialCapacity);
    heap->Comparator = Comparator;
    return (PVectorHandle) heap;
}

PVectorHeapHandle PVectorHeap_Heapify (PVectorHandle vectorHandle,
        int (*Comparator) (const void *first, const void *second))
{
    PVectorHeap *heap = malloc (sizeof (PVectorHeap));
    heap->vector = vectorHandle;
    heap->Comparator = Comparator;

    uint size = PVector_Size (vectorHandle);
    for (uint index = size / 2; index < size; --index)
    {
        PVectorHeap_SiftDown (heap, index);
    }

    return (PVectorHandle) heap;
}

void PVectorHeap_Destruct (PVectorHeapHandle handle, void (*DestructCallback) (void **item))
{
    PVectorHeap *heap = (PVectorHeap *) handle;
    PVector_Destruct (heap->vector, DestructCallback);
    free (heap);
}

uint PVectorHeap_Size (PVectorHeapHandle handle)
{
    PVectorHeap *heap = (PVectorHeap *) handle;
    return PVector_Size (heap->vector);
}

const void *PVectorHeap_Top (PVectorHeapHandle handle)
{
    if (PVectorHeap_Size (handle) > 0)
    {
        PVectorHeap *heap = (PVectorHeap *) handle;
        return *PVectorIterator_ValueAt (PVector_Begin (heap->vector));
    }
    else
    {
        return NULL;
    }
}

void PVectorHeap_Pop (PVectorHeapHandle handle)
{
    if (PVectorHeap_Size (handle) > 0)
    {
        PVectorHeap *heap = (PVectorHeap *) handle;
        *PVectorIterator_ValueAt (PVector_Begin (heap->vector)) = *PVectorIterator_ValueAt (
                PVectorIterator_Previous (PVector_End (heap->vector)));

        PVector_Erase (heap->vector, PVectorIterator_Previous (PVector_End (heap->vector)));
        PVectorHeap_SiftDown (heap, 0);
    }
}

void PVectorHeap_Push (PVectorHeapHandle handle, void *value)
{
    PVectorHeap *heap = (PVectorHeap *) handle;
    PVector_Insert (heap->vector, PVector_End (heap->vector), value);
    PVectorHeap_SiftUp (heap, PVector_Size (heap->vector) - 1);
}
