#include "PVectorHeap.h"
#include "Utils.h"
#include "PVector.h"
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
    VirtualHandle vector;
    ulint size;
    IIterableContainer *IIterable;
    IBiDirectionalIterator *IIterator;
    IMutableContainer *IMutable;
    lint (*Comparator) (const void *first, const void *second);
    char reverseComparator;
} PVectorHeap;

char PVectorHeap_DoCompare (PVectorHeap *heap, const void *first, const void *second)
{
    lint result = heap->Comparator (first, second);
    return heap->reverseComparator ? result < 0 : result > 0;
}

static void PVectorHeap_SiftDown (PVectorHeap *heap, ulint elementIndex)
{
    if (elementIndex >= heap->size)
    {
        return;
    }

    char mustSift = 1;
    void *valueToSift = *(heap->IIterator->Value (heap->IIterable->At (heap->vector, elementIndex)));

    while (mustSift)
    {
        mustSift = 0;
        ulint leftChild = elementIndex * 2 + 1;
        ulint rightChild = elementIndex * 2 + 2;
        ulint size = heap->size;

        void **current = heap->IIterator->Value (heap->IIterable->At (heap->vector, elementIndex));
        if (rightChild > size)
        {
            *current = valueToSift;
            return;
        }


        void **left = heap->IIterator->Value (heap->IIterable->At (heap->vector, leftChild));
        if (rightChild == size)
        {
            if (PVectorHeap_DoCompare (heap, *left, valueToSift))
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
            void **right = heap->IIterator->Value (heap->IIterable->At (heap->vector, rightChild));
            void **better = NULL;
            ulint betterIndex;

            if (PVectorHeap_DoCompare (heap, *right, *left))
            {
                better = right;
                betterIndex = rightChild;
            }
            else
            {
                better = left;
                betterIndex = leftChild;
            }

            if (PVectorHeap_DoCompare (heap, *better, valueToSift))
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

static void PVectorHeap_SiftUp (PVectorHeap *heap, ulint elementIndex)
{
    if (elementIndex >= heap->size)
    {
        return;
    }

    char mustSift = 1;
    void *valueToSift = *(heap->IIterator->Value (heap->IIterable->At (heap->vector, elementIndex)));

    while (mustSift)
    {
        mustSift = 0;
        void **current = heap->IIterator->Value (heap->IIterable->At (heap->vector, elementIndex));
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

        void **parent = heap->IIterator->Value (heap->IIterable->At (heap->vector, parentIndex));
        if (PVectorHeap_DoCompare (heap, valueToSift, *parent))
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

PVectorHeapHandle PVectorHeap_Create (ulint initialCapacity, lint (*Comparator) (const void *first, const void *second))
{
    PVectorHeap *heap = malloc (sizeof (PVectorHeap));
    heap->vector = PVector_Create (initialCapacity);
    heap->size = 0;

    heap->IIterable = PVector_AsIIterableContainer ();
    heap->IIterator = PVectorIterator_AsIBiDirectionalIterator ();
    heap->IMutable = PVector_AsIMutableContainer ();

    heap->Comparator = Comparator;
    heap->reverseComparator = 0;
    return (PVectorHandle) heap;
}

PVectorHeapHandle PVectorHeap_Heapify (VirtualHandle vector, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second), char reverseComparator)
{
    PVectorHeap *heap = malloc (sizeof (PVectorHeap));
    heap->vector = vector;
    heap->size = ISized->Size (vector);

    heap->IIterable = IIterable;
    heap->IIterator = IIterator;
    heap->IMutable = IMutable;
    heap->Comparator = Comparator;
    heap->reverseComparator = reverseComparator;

    ulint size = heap->size;
    for (ulint index = size / 2; index < size; --index)
    {
        PVectorHeap_SiftDown (heap, index);
    }

    return (PVectorHandle) heap;
}

void PVectorHeap_Destruct (PVectorHeapHandle handle,
        void (*BaseDestructor) (VirtualHandle vector, void (*DestructCallback) (void **item)),
        void (*DestructCallback) (void **item))
{
    PVectorHeap *heap = (PVectorHeap *) handle;
    BaseDestructor (heap->vector, DestructCallback);
    free (heap);
}

ulint PVectorHeap_Size (PVectorHeapHandle handle)
{
    PVectorHeap *heap = (PVectorHeap *) handle;
    return heap->size;
}

const void *PVectorHeap_Top (PVectorHeapHandle handle)
{
    if (PVectorHeap_Size (handle) > 0)
    {
        PVectorHeap *heap = (PVectorHeap *) handle;
        return *heap->IIterator->Value (heap->IIterable->Begin (heap->vector));
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
        *heap->IIterator->Value (heap->IIterable->Begin (heap->vector)) = *heap->IIterator->Value (
                heap->IIterable->At (heap->vector, heap->size - 1));

        heap->IMutable->Erase (heap->vector, heap->IIterable->At (heap->vector, heap->size - 1));
        heap->size -= 1;
        PVectorHeap_SiftDown (heap, 0);
    }
}

void PVectorHeap_Push (PVectorHeapHandle handle, void *value)
{
    PVectorHeap *heap = (PVectorHeap *) handle;
    VirtualHandle insertBefore = heap->size == 0 ? heap->IIterable->Begin (heap->vector) :
                                 heap->IIterator->Next (heap->IIterable->At (heap->vector, heap->size - 1));

    heap->IMutable->Insert (heap->vector, insertBefore, value);
    heap->size += 1;
    PVectorHeap_SiftUp (heap, heap->size - 1);
}

void PVectorHeap_ReverseComparator (PVectorHeapHandle handle)
{
    PVectorHeap *heap = (PVectorHeap *) handle;
    heap->reverseComparator = heap->reverseComparator == 0 ? 1 : 0;
}

ISizedContainer *PVectorHeap_AsISizedContainer ()
{
    return &PVectorHeap_ISizedContainer;
}

IOrganizerContainer *PVectorHeap_AsIOrganizerContainer ()
{
    return &PVectorHeap_IOrganizerContainer;
}
