#include "PODArraySort.h"
#include <CContainers/PODMemory.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

static void PODArrayHeapSort_SiftDown (char *begin, const char *end, ulint elementIndex, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    ulint size = end - begin;
    if (elementIndex >= size)
    {
        return;
    }

    char mustSift = 1;
    char *valueToSift = malloc (elementSize);
    memcpy (valueToSift, begin + elementIndex * elementSize, elementSize);

    while (mustSift)
    {
        mustSift = 0;
        ulint leftChild = elementIndex * 2 + 1;
        ulint rightChild = elementIndex * 2 + 2;

        char *current = begin + elementIndex * elementSize;
        char *left = begin + (elementIndex * 2 + 1) * elementSize;
        char *right = begin + (elementIndex * 2 + 2) * elementSize;

        if (right > end)
        {
            memcpy (current, valueToSift, elementSize);
            break;
        }

        if (right == end)
        {
            if (Comparator (left, valueToSift) < 0)
            {
                memcpy (current, left, elementSize);
                memcpy (left, valueToSift, elementSize);
            }
            else
            {
                memcpy (current, valueToSift, elementSize);
            }
        }
        else
        {
            char *better = NULL;
            ulint betterIndex;

            if (Comparator (right, left) < 0)
            {
                better = right;
                betterIndex = rightChild;
            }
            else
            {
                better = left;
                betterIndex = leftChild;
            }

            if (Comparator (better, valueToSift) < 0)
            {
                memcpy (current, better, elementSize);
                elementIndex = betterIndex;
                mustSift = 1;
            }
            else
            {
                memcpy (current, valueToSift, elementSize);
            }
        }
    }

    free (valueToSift);
}

void PODArrayHeapSort (char *begin, const char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    ulint size = (end - begin) / elementSize;
    for (ulint index = size / 2; index < size; --index)
    {
        PODArrayHeapSort_SiftDown (begin, end, index, elementSize, Comparator);
    }

    while (size > 0)
    {
        PODMemory_Swap (begin, begin + (size - 1) * elementSize, elementSize);
        --size;
        PODArrayHeapSort_SiftDown (begin, begin + size * elementSize, 0, elementSize, Comparator);
    }
}

void PODArrayMergeSortedParts (char *begin, char *middle, char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    ulint leftPartSizeInBytes = middle - begin;
    ulint rightPartSizeInBytes = end - middle;

    char *leftPart = malloc (leftPartSizeInBytes);
    char *rightPart = malloc (rightPartSizeInBytes);

    memcpy (leftPart, begin, leftPartSizeInBytes);
    memcpy (rightPart, middle, rightPartSizeInBytes);

    char *iterator = begin;
    char *leftIterator = leftPart;
    char *rightIterator = rightPart;

    char *leftEnd = leftPart + leftPartSizeInBytes;
    char *rightEnd = rightPart + rightPartSizeInBytes;

    char moreLeft = leftIterator != leftEnd;
    char moreRight = rightIterator != rightEnd;

    while (moreLeft || moreRight)
    {
        char *best = NULL;
        if (!moreRight)
        {
            best = leftIterator;
            leftIterator += elementSize;
        }
        else if (!moreLeft)
        {
            best = rightIterator;
            rightIterator += elementSize;
        }
        else
        {
            if (Comparator (leftIterator, rightIterator) > 0)
            {
                best = leftIterator;
                leftIterator += elementSize;
            }
            else
            {
                best = rightIterator;
                rightIterator += elementSize;
            }
        }

        memcpy (iterator, best, elementSize);
        iterator += elementSize;

        moreLeft = leftIterator != leftEnd;
        moreRight = rightIterator != rightEnd;
    }

    free (leftPart);
    free (rightPart);
}

void PODArrayInplaceMergeSortedParts (char *begin, char *middle, char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    ulint leftPartSize = (middle - begin) / elementSize;
    ulint rightPartSize = (end - middle) / elementSize;
    
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

            if (Comparator (begin + leftIndex * elementSize, middle + rightIndex * elementSize) >= 0)
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
        
        if (Comparator (begin + leftIndex * elementSize, middle + rightIndex * elementSize) > 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }

    if (mid == 0)
    {
        return;
    }

    char *leftBegin = begin + (leftPartSize - mid) * elementSize;
    char *rightEnd = middle + mid * elementSize;

    PODMemory_Swap (leftBegin, middle, mid * elementSize);
    if (mid != leftPartSize)
    {
        PODArrayInplaceMergeSortedParts (begin, leftBegin, middle, elementSize, Comparator);
    }

    if (mid != rightPartSize)
    {
        PODArrayInplaceMergeSortedParts (middle, rightEnd, end, elementSize, Comparator);
    }
}

void PODArrayMergeSortInternal (char *begin, const char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second),
        void (*Merge) (char *begin, char *middle, char *end, ulint elementSize,
                lint (*Comparator) (const char *first, const char *second)))
{
    ulint size = (end - begin) / elementSize;
    for (ulint subArraySize = 1; subArraySize < size; subArraySize *= 2)
    {
        char *partitionBegin = begin;
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
            char *partitionMiddle = partitionBegin + leftSize * elementSize;
            char *partitionEnd = partitionMiddle + rightSize * elementSize;

            Merge (partitionBegin, partitionMiddle, partitionEnd, elementSize, Comparator);
            partitionBegin = partitionEnd;
        }
    }
}

void PODArrayMergeSort (char *begin, const char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    PODArrayMergeSortInternal (begin, end, elementSize, Comparator, PODArrayMergeSortedParts);
}

void PODArrayInplaceMergeSort (char *begin, const char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    PODArrayMergeSortInternal (begin, end, elementSize, Comparator, PODArrayInplaceMergeSortedParts);
}

char *PODArrayQuickSortPartition (char *begin, char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    char *pivotIterator = end - elementSize;
    char *nextSmallerIterator = begin;
    char *iterator = begin;

    while (iterator != pivotIterator)
    {
        if (Comparator (iterator, pivotIterator) >= 0)
        {
            PODMemory_Swap (iterator, nextSmallerIterator, elementSize);
            nextSmallerIterator += elementSize;
        }

        iterator += elementSize;
    }

    PODMemory_Swap (pivotIterator, nextSmallerIterator, elementSize);
    return nextSmallerIterator;
}

void PODArrayQuickSort (char *begin, char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    // TODO: Maybe switch back to recursive implementation to save memory?
    ulint size = (end - begin) / elementSize;
    char **stack = malloc (sizeof (ulint) * size * 2);

    ulint stackSize = 0;
    stack [stackSize++] = begin;
    stack [stackSize++] = end;

    while (stackSize > 0)
    {
        end = stack [--stackSize];
        begin = stack [--stackSize];
        char *middle = PODArrayQuickSortPartition (begin, end, elementSize, Comparator);

        if (begin < middle)
        {
            stack [stackSize++] = begin;
            stack [stackSize++] = middle;
        }

        if (middle + elementSize < end)
        {
            stack [stackSize++] = middle + elementSize;
            stack [stackSize++] = end;
        }
    }
}

void PODArrayIntroSortInternal (char *begin, char *end, ulint depthLimit, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    if (depthLimit == 0)
    {
        PODArrayHeapSort (begin, end, elementSize, Comparator);
    }
    else
    {
        char *middle = PODArrayQuickSortPartition (begin, end, elementSize, Comparator);
        if (begin != middle)
        {
            PODArrayIntroSortInternal (begin, middle, depthLimit - 1, elementSize, Comparator);
        }

        if (middle + elementSize != end)
        {
            PODArrayIntroSortInternal (middle + elementSize, end, depthLimit - 1, elementSize, Comparator);
        }
    }
}

void PODArrayIntroSort (char *begin, char *end, ulint elementSize,
        lint (*Comparator) (const char *first, const char *second))
{
    ulint size = end - begin;
    ulint depthLimit = (ulint) log (size);
    PODArrayIntroSortInternal (begin, end, depthLimit, elementSize, Comparator);
}
