#include "Sort.h"
#include <stdlib.h>
#include <minmax.h>
#include <CContainers/PVector.h>
#include <CContainers/PHeap.h>
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
    PHeapHandle heap = PHeap_Heapify (
            container, ISized, IIterable, IIterator, &ICustomMutable, Comparator, 1);

    VirtualHandle iterator = IIterator->Previous (IIterable->End (container));
    do
    {
        const void *value = PHeap_Top (heap);
        PHeap_Pop (heap);
        // Const qualifier discarded not by mistake!
        *(IIterator->Value (iterator)) = value;

        if (iterator == IIterable->Begin (container))
        {
            break;
        }

        iterator = IIterator->Previous (iterator);
    }
    while (1);

    PHeap_Destruct (heap, HeapSort_DummyDestruct, ContainerCallback_NoAction);
}

void MergeSortedParts (VirtualHandle begin, VirtualHandle middle, VirtualHandle end,
        ulint leftPartSize, ulint rightPartSize, IOneDirectionIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second))
{
    void **leftPart = malloc (sizeof (void *) * leftPartSize);
    void **rightPart = malloc (sizeof (void *) * rightPartSize);

    VirtualHandle iterator = begin;
    ulint index = 0;

    while (iterator != middle)
    {
        leftPart [index] = *IIterator->Value (iterator);
        iterator = IIterator->Next (iterator);
        ++index;
    }

    index = 0;
    while (iterator != end)
    {
        rightPart [index] = *IIterator->Value (iterator);
        iterator = IIterator->Next (iterator);
        ++index;
    }

    iterator = begin;
    ulint leftPartIndex = 0;
    ulint rightPartIndex = 0;

    while (leftPartIndex < leftPartSize || rightPartIndex < rightPartSize)
    {
        void *best = NULL;
        if (rightPartIndex >= rightPartSize)
        {
            best = leftPart [leftPartIndex];
            ++leftPartIndex;
        }
        else if (leftPartIndex >= leftPartSize)
        {
            best = rightPart [rightPartIndex];
            ++rightPartIndex;
        }
        else
        {
            void *left = leftPart [leftPartIndex];
            void *right = rightPart [rightPartIndex];

            if (Comparator (left, right) > 0)
            {
                best = left;
                ++leftPartIndex;
            }
            else
            {
                best = right;
                ++rightPartIndex;
            }
        }

        *IIterator->Value (iterator) = best;
        iterator = IIterator->Next (iterator);
    }

    free (leftPart);
    free (rightPart);
}

void MergeSort (VirtualHandle begin, VirtualHandle end, ulint size, IOneDirectionIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second))
{
    for (ulint subArraySize = 1; subArraySize < size; subArraySize *= 2)
    {
        VirtualHandle partitionBegin = begin;
        for (ulint partitionBeginIndex = 0; partitionBeginIndex < size - 1; partitionBeginIndex += subArraySize * 2)
        {
            ulint partitionEndIndex = min (partitionBeginIndex + subArraySize * 2, size);
            ulint actualPartitionSize = partitionEndIndex - partitionBeginIndex;
            ulint leftSize = min (subArraySize, actualPartitionSize);
            ulint rightSize = actualPartitionSize - leftSize;

            VirtualHandle partitionMiddle = IIterator->Jump (partitionBegin, leftSize);
            VirtualHandle partitionEnd = IIterator->Jump (partitionMiddle, rightSize);

            MergeSortedParts (partitionBegin, partitionMiddle, partitionEnd,
                    leftSize, rightSize, IIterator, Comparator);

            partitionBegin = partitionEnd;
        }
    }
}
