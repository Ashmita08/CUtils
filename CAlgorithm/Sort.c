#include "Sort.h"
#include <stdlib.h>
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

void InplaceMergeSortedParts (VirtualHandle begin, VirtualHandle middle, VirtualHandle end,
        ulint leftPartSize, ulint rightPartSize, IBiDirectionalIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second))
{
    if (leftPartSize == 0 || rightPartSize == 0)
    {
        return;
    }

    ulint size = leftPartSize < rightPartSize ? leftPartSize : rightPartSize;
    ulint low = 0;
    ulint high = size - 1;
    ulint mid = 0;

    while (high < size)
    {
        if (low > high)
        {
            ulint rightIndex = high;
            ulint leftIndex = leftPartSize - 1 - high;

            VirtualHandle leftIterator = IIterator->Jump (begin, leftIndex);
            VirtualHandle rightIterator = IIterator->Jump (middle, rightIndex);

            if (Comparator (*IIterator->Value (leftIterator), *IIterator->Value (rightIterator)) >= 0)
            {
                mid = high;
            }
            else
            {
                mid = high + 1;
            }

            break;
        }

        mid = low + (high - low) / 2;
        ulint rightIndex = mid;
        ulint leftIndex = leftPartSize - 1 - mid;

        VirtualHandle leftIterator = IIterator->Jump (begin, leftIndex);
        VirtualHandle rightIterator = IIterator->Jump (middle, rightIndex);

        if (Comparator (*IIterator->Value (leftIterator), *IIterator->Value (rightIterator)) > 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }

    ulint rotationSize = 2 * mid;
    if (rotationSize == 0)
    {
        return;
    }

    VirtualHandle leftBegin = IIterator->Jump (begin, leftPartSize - mid);
    VirtualHandle rightEnd = IIterator->Jump (middle, mid);

    VirtualHandle leftIterator = leftBegin;
    VirtualHandle rightIterator = middle;

    while (leftIterator != middle)
    {
        void **left = IIterator->Value (leftIterator);
        void **right = IIterator->Value (rightIterator);
        void *temp = *left;

        *left = *right;
        *right = temp;

        leftIterator = IIterator->Next (leftIterator);
        rightIterator = IIterator->Next (rightIterator);
    }

    if (rotationSize / 2 != leftPartSize)
    {
        ulint nextLeftPartSize = leftPartSize - rotationSize / 2;
        ulint nextRightPartSize = rotationSize / 2;
        InplaceMergeSortedParts (begin, leftBegin, middle, nextLeftPartSize, nextRightPartSize, IIterator, Comparator);
    }

    if (rotationSize / 2 != rightPartSize)
    {
        ulint nextLeftPartSize = rotationSize / 2;
        ulint nextRightPartSize = rightPartSize - rotationSize / 2;
        InplaceMergeSortedParts (middle, rightEnd, end, nextLeftPartSize, nextRightPartSize, IIterator, Comparator);
    }
}

void MergeSortInternal (VirtualHandle begin, VirtualHandle end, ulint size, IOneDirectionIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second), char inplace)
{
    // WARNING: Use only Next and Jump from IIterator interface!
    for (ulint subArraySize = 1; subArraySize < size; subArraySize *= 2)
    {
        VirtualHandle partitionBegin = begin;
        for (ulint partitionBeginIndex = 0; partitionBeginIndex < size - 1; partitionBeginIndex += subArraySize * 2)
        {
            ulint partitionEndIndex = partitionBeginIndex + subArraySize * 2;
            if (partitionEndIndex > size)
            {
                partitionEndIndex = size;
            }

            ulint actualPartitionSize = partitionEndIndex - partitionBeginIndex;
            ulint leftSize = subArraySize;

            if (leftSize > actualPartitionSize)
            {
                continue;
            }

            ulint rightSize = actualPartitionSize - leftSize;
            VirtualHandle partitionMiddle = IIterator->Jump (partitionBegin, leftSize);
            VirtualHandle partitionEnd = IIterator->Jump (partitionMiddle, rightSize);

            if (inplace)
            {
                InplaceMergeSortedParts (partitionBegin, partitionMiddle, partitionEnd,
                        leftSize, rightSize, IIterator, Comparator);
            }
            else
            {
                MergeSortedParts (partitionBegin, partitionMiddle, partitionEnd,
                        leftSize, rightSize, IIterator, Comparator);
            }

            partitionBegin = partitionEnd;
        }
    }
}

void MergeSort (VirtualHandle begin, VirtualHandle end, ulint size, IOneDirectionIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second))
{
    MergeSortInternal (begin, end, size, IIterator, Comparator, 0);
}

void InplaceMergeSort (VirtualHandle begin, VirtualHandle end, ulint size, IBiDirectionalIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second))
{
    MergeSortInternal (begin, end, size, IIterator, Comparator, 1);
}
