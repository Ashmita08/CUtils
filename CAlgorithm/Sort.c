#include "Sort.h"
#include <stdlib.h>
#include <CContainers/PVector.h>
#include <CContainers/PVectorHeap.h>
#include <CContainers/Utils.h>

static VirtualHandle HeapSort_DummyErase (VirtualHandle container, VirtualHandle iterator)
{

}

static void HeapSort_DummyDestruct (VirtualHandle container, void (*Callback) (void **item))
{

}

void HeapSort (VirtualHandle container, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second))
{
    IMutableContainer ICustomMutable = *IMutable;
    ICustomMutable.Erase = HeapSort_DummyErase;
    PVectorHeapHandle heap = PVectorHeap_Heapify (
            container, ISized, IIterable, IIterator, &ICustomMutable, Comparator, 1);

    VirtualHandle iterator = IIterator->Previous (IIterable->End (container));
    do
    {
        const void *value = PVectorHeap_Top (heap);
        PVectorHeap_Pop (heap);
        // Const qualifier discarded not by mistake!
        *(IIterator->Value (iterator)) = value;

        if (iterator == IIterable->Begin (container))
        {
            break;
        }

        iterator = IIterator->Previous (iterator);
    }
    while (1);

    PVectorHeap_Destruct (heap, HeapSort_DummyDestruct, ContainerCallback_NoAction);
}

void MergeSortedParts (VirtualHandle begin, VirtualHandle middle, VirtualHandle end,
        IOneDirectionIterator *IIterator, lint (*Comparator) (const void *first, const void *second))
{
#define MERGE_SORTED_PARTS_RESERVE_PART 32
    PVectorHandle leftPart = PVector_Create (MERGE_SORTED_PARTS_RESERVE_PART);
    PVectorHandle rightPart = PVector_Create (MERGE_SORTED_PARTS_RESERVE_PART);

    VirtualHandle iterator = begin;
    while (iterator != middle)
    {
        PVector_Insert (leftPart, PVector_End (leftPart), *IIterator->Value (iterator));
        iterator = IIterator->Next (iterator);
    }

    while (iterator != end)
    {
        PVector_Insert (rightPart, PVector_End (rightPart), *IIterator->Value (iterator));
        iterator = IIterator->Next (iterator);
    }

    iterator = begin;
    VirtualHandle leftIterator = PVector_Begin (leftPart);
    VirtualHandle rightIterator = PVector_Begin (rightPart);

    while (leftIterator != PVector_End (leftPart) || rightIterator != PVector_End (rightPart))
    {
        void *best = NULL;
        if (leftIterator == PVector_End (leftPart))
        {
            best = *PVectorIterator_ValueAt (rightIterator);
            rightIterator = PVectorIterator_Next (rightIterator);
        }
        else if (rightIterator == PVector_End (rightPart))
        {
            best = *PVectorIterator_ValueAt (leftIterator);
            leftIterator = PVectorIterator_Next (leftIterator);
        }
        else
        {
            void *left = *PVectorIterator_ValueAt (leftIterator);
            void *right = *PVectorIterator_ValueAt (rightIterator);

            if (Comparator (left, right) > 0)
            {
                best = left;
                leftIterator = PVectorIterator_Next (leftIterator);
            }
            else
            {
                best = right;
                rightIterator = PVectorIterator_Next (rightIterator);
            }
        }

        *IIterator->Value (iterator) = best;
        iterator = IIterator->Next (iterator);
    }

    PVector_Destruct (leftPart, ContainerCallback_NoAction);
    PVector_Destruct (rightPart, ContainerCallback_NoAction);
}

void MergeSort (VirtualHandle begin, VirtualHandle end, ulint size, IOneDirectionIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second))
{
    VirtualHandle middle = IIterator->Jump (begin, size / 2);
    if (size / 2 > 0)
    {
        MergeSort (begin, middle, size / 2, IIterator, Comparator);
        MergeSort (middle, end, size - size / 2, IIterator, Comparator);
        MergeSortedParts (begin, middle, end, IIterator, Comparator);
    }
}
