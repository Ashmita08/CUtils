#include "PHeap.h"
#include "Utils.h"
#include "PVector.h"
#include "Errors.h"
#include <stdlib.h>

ISizedContainer PHeap_ISizedContainer =
        {
            PHeap_Size
        };

IOrganizerContainer PHeap_IOrganizerContainer =
        {
            PHeap_Push,
            PHeap_Top,
            PHeap_Pop
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
} PHeap;

char PHeap_DoCompare (PHeap *heap, const void *first, const void *second)
{
    lint result = heap->Comparator (first, second);
    return heap->reverseComparator ? result < 0 : result > 0;
}

static void PHeap_SiftDown (PHeap *heap, ulint elementIndex)
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
            if (PHeap_DoCompare (heap, *left, valueToSift))
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

            if (PHeap_DoCompare (heap, *right, *left))
            {
                better = right;
                betterIndex = rightChild;
            }
            else
            {
                better = left;
                betterIndex = leftChild;
            }

            if (PHeap_DoCompare (heap, *better, valueToSift))
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

static void PHeap_SiftUp (PHeap *heap, ulint elementIndex)
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
        if (PHeap_DoCompare (heap, valueToSift, *parent))
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

PHeapHandle PHeap_Create (ulint initialCapacity, lint (*Comparator) (const void *first, const void *second))
{
    PHeap *heap = malloc (sizeof (PHeap));
    heap->vector = PVector_Create (initialCapacity);
    heap->size = 0;

    heap->IIterable = PVector_AsIIterableContainer ();
    heap->IIterator = PVectorIterator_AsIBiDirectionalIterator ();
    heap->IMutable = PVector_AsIMutableContainer ();

    heap->Comparator = Comparator;
    heap->reverseComparator = 0;
    return (PVectorHandle) heap;
}

PHeapHandle PHeap_Heapify (VirtualHandle vector, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second), char reverseComparator)
{
    PHeap *heap = malloc (sizeof (PHeap));
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
        PHeap_SiftDown (heap, index);
    }

    return (PVectorHandle) heap;
}

void PHeap_Destruct (PHeapHandle handle,
        void (*BaseDestructor) (VirtualHandle vector, void (*DestructCallback) (void **item)),
        void (*DestructCallback) (void **item))
{
    PHeap *heap = (PHeap *) handle;
    BaseDestructor (heap->vector, DestructCallback);
    free (heap);
}

ulint PHeap_Size (PHeapHandle handle)
{
    PHeap *heap = (PHeap *) handle;
    return heap->size;
}

const void *PHeap_Top (PHeapHandle handle)
{
    if (PHeap_Size (handle) > 0)
    {
        PHeap *heap = (PHeap *) handle;
        return *heap->IIterator->Value (heap->IIterable->Begin (heap->vector));
    }
    else
    {
        return NULL;
    }
}

void PHeap_Pop (PHeapHandle handle)
{
    if (PHeap_Size (handle) > 0)
    {
        PHeap *heap = (PHeap *) handle;
        *heap->IIterator->Value (heap->IIterable->Begin (heap->vector)) = *heap->IIterator->Value (
                heap->IIterable->At (heap->vector, heap->size - 1));

        heap->IMutable->Erase (heap->vector, heap->IIterable->At (heap->vector, heap->size - 1));
        heap->size -= 1;
        PHeap_SiftDown (heap, 0);
    }
}

void PHeap_Push (PHeapHandle handle, void *value)
{
    PHeap *heap = (PHeap *) handle;
    VirtualHandle insertBefore = heap->size == 0 ? heap->IIterable->Begin (heap->vector) :
                                 heap->IIterator->Next (heap->IIterable->At (heap->vector, heap->size - 1));

    heap->IMutable->Insert (heap->vector, insertBefore, value);
    heap->size += 1;
    PHeap_SiftUp (heap, heap->size - 1);
}

void PHeap_ReverseComparator (PHeapHandle handle)
{
    PHeap *heap = (PHeap *) handle;
    heap->reverseComparator = heap->reverseComparator == 0 ? 1 : 0;
}

ISizedContainer *PHeap_AsISizedContainer ()
{
    return &PHeap_ISizedContainer;
}

IOrganizerContainer *PHeap_AsIOrganizerContainer ()
{
    return &PHeap_IOrganizerContainer;
}
