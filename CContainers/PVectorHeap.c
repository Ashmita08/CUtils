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
    ISizedContainer *ISized;
    IIterableContainer *IIterable;
    IBiDirectionalIterator *IIterator;
    IMutableContainer *IMutable;
    lint (*Comparator) (const void *first, const void *second);
} PVectorHeap;

static void PVectorHeap_SiftDown (PVectorHeap *heap, ulint elementIndex)
{
    ulint leftChild = elementIndex * 2 + 1;
    ulint rightChild = elementIndex * 2 + 2;
    ulint size = heap->ISized->Size (heap->vector);

    if (rightChild > size)
    {
        return;
    }

    void **current = heap->IIterator->Value (heap->IIterable->At (heap->vector, elementIndex));
    void **left = heap->IIterator->Value (heap->IIterable->At (heap->vector, leftChild));

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
        void **right = heap->IIterator->Value (heap->IIterable->At (heap->vector, rightChild));
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

        if (heap->Comparator (*better, *current) > 0)
        {
            void *temp = *current;
            *current = *better;
            *better = temp;
            PVectorHeap_SiftDown (heap, betterIndex);
        }
    }
}

static void PVectorHeap_SiftUp (PVectorHeap *heap, ulint elementIndex)
{
    if (elementIndex == 0)
    {
        return;
    }

    ulint parentIndex = elementIndex / 2;
    if (elementIndex % 2 == 0)
    {
        --parentIndex;
    }

    void **current = heap->IIterator->Value (heap->IIterable->At (heap->vector, elementIndex));;
    void **parent = heap->IIterator->Value (heap->IIterable->At (heap->vector, parentIndex));;

    if (heap->Comparator (*current, *parent) > 0)
    {
        void *temp = *current;
        *current = *parent;
        *parent = temp;
        PVectorHeap_SiftUp (heap, parentIndex);
    }
}

PVectorHeapHandle PVectorHeap_Create (ulint initialCapacity, lint (*Comparator) (const void *first, const void *second))
{
    PVectorHeap *heap = malloc (sizeof (PVectorHeap));
    heap->vector = PVector_Create (initialCapacity);
    heap->ISized = PVector_AsISizedContainer ();
    heap->IIterable = PVector_AsIIterableContainer ();
    heap->IIterator = PVectorIterator_AsIBiDirectionalIterator ();
    heap->IMutable = PVector_AsIMutableContainer ();
    heap->Comparator = Comparator;
    return (PVectorHandle) heap;
}

PVectorHeapHandle PVectorHeap_Heapify (VirtualHandle vector, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second))
{
    PVectorHeap *heap = malloc (sizeof (PVectorHeap));
    heap->vector = vector;
    heap->ISized = ISized;
    heap->IIterable = IIterable;
    heap->IIterator = IIterator;
    heap->IMutable = IMutable;
    heap->Comparator = Comparator;

    ulint size = ISized->Size (heap->vector);
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
    return heap->ISized->Size (heap->vector);
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
                heap->IIterator->Previous (heap->IIterable->End (heap->vector)));

        heap->IMutable->Erase (heap->vector, heap->IIterator->Previous (heap->IIterable->End (heap->vector)));
        PVectorHeap_SiftDown (heap, 0);
    }
}

void PVectorHeap_Push (PVectorHeapHandle handle, void *value)
{
    PVectorHeap *heap = (PVectorHeap *) handle;
    heap->IMutable->Insert (heap->vector, heap->IIterable->End (heap->vector), value);
    PVectorHeap_SiftUp (heap, PVectorHeap_Size (handle) - 1);
}

ISizedContainer *PVectorHeap_AsISizedContainer ()
{
    return &PVectorHeap_ISizedContainer;
}

IOrganizerContainer *PVectorHeap_AsIOrganizerContainer ()
{
    return &PVectorHeap_IOrganizerContainer;
}
