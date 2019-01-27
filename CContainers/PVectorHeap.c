#include "PVectorHeap.h"
#include "Utils.h"
#include "Errors.h"
#include <stdlib.h>

ISizedContainer PVectorHeap_ISizedContainer =
        {
                PVectorHeap_Size
        };

IOrganizerContainer PVectorHeap_IOrganizerContainer =
        {
                PVectorHeap_Push,
                PVectorHeap_Top,
                PVectorHeap_Pop
        };

typedef struct
{
    PVectorHandle vector;
    lint (*Comparator) (const void *first, const void *second);
} PVectorHeap;

static void PVectorHeap_SiftDown (PVectorHeap *heap, uint elementIndex)
{
    ulint size = PVector_Size (heap->vector);
    if (elementIndex >= size)
    {
        return;
    }

    char mustSift = 1;
    void *valueToSift = *PVectorIterator_ValueAt (PVector_At (heap->vector, elementIndex));

    while (mustSift)
    {
        mustSift = 0;
        ulint leftChild = elementIndex * 2 + 1;
        ulint rightChild = elementIndex * 2 + 2;

        void **current = PVectorIterator_ValueAt (PVector_At (heap->vector, elementIndex));
        if (rightChild > size)
        {
            *current = valueToSift;
            return;
        }


        void **left = PVectorIterator_ValueAt (PVector_At (heap->vector, leftChild));
        if (rightChild == size)
        {
            if (heap->Comparator (*left, valueToSift) > 0)
            {
                *current = *left;
                *left = valueToSift;
            }
            else
            {
                *current = valueToSift;
            }
        }
        else
        {
            void **right = PVectorIterator_ValueAt (PVector_At (heap->vector, rightChild));
            void **better = NULL;
            ulint betterIndex;

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

            if (heap->Comparator (*better, valueToSift) > 0)
            {
                *current = *better;
                elementIndex = betterIndex;
                mustSift = 1;
            }
            else
            {
                *current = valueToSift;
            }
        }
    }
}

static void PVectorHeap_SiftUp (PVectorHeap *heap, uint elementIndex)
{
    ulint size = PVector_Size (heap->vector);
    if (elementIndex >= size)
    {
        return;
    }

    char mustSift = 1;
    void *valueToSift = *PVectorIterator_ValueAt (PVector_At (heap->vector, elementIndex));

    while (mustSift)
    {
        mustSift = 0;
        void **current = PVectorIterator_ValueAt (PVector_At (heap->vector, elementIndex));
        if (elementIndex == 0)
        {
            *current = valueToSift;
            return;
        }

        ulint parentIndex = elementIndex / 2;
        if (elementIndex % 2 == 0)
        {
            --parentIndex;
        }

        void **parent = PVectorIterator_ValueAt (PVector_At (heap->vector, parentIndex));
        if (heap->Comparator (valueToSift, *parent) > 0)
        {
            *current = *parent;
            elementIndex = parentIndex;
            mustSift = 1;
        }
        else
        {
            *current = valueToSift;
        }
    }
}

PVectorHeapHandle PVectorHeap_Create (uint initialCapacity, lint (*Comparator) (const void *first, const void *second))
{
    PVectorHeap *heap = malloc (sizeof (PVectorHeap));
    heap->vector = PVector_Create (initialCapacity);
    heap->Comparator = Comparator;
    return (PVectorHandle) heap;
}

PVectorHeapHandle PVectorHeap_Heapify (PVectorHandle vectorHandle,
        lint (*Comparator) (const void *first, const void *second))
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

ulint PVectorHeap_Size (PVectorHeapHandle handle)
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

ISizedContainer *PVectorHeap_AsISizedContainer ()
{
    return &PVectorHeap_ISizedContainer;
}

IOrganizerContainer *PVectorHeap_AsIOrganizerContainer ()
{
    return &PVectorHeap_IOrganizerContainer;
}
