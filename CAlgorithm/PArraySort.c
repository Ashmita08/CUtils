#include "PArraySort.h"
#include <stdlib.h>
#include <math.h>

static void PArrayHeapSort_SiftDown (void **begin, void **end, ulint elementIndex,
        lint (*Comparator) (const void *first, const void *second))
{
    ulint size = end - begin;
    if (elementIndex >= size)
    {
        return;
    }

    char mustSift = 1;
    void *valueToSift = begin [elementIndex];

    while (mustSift)
    {
        mustSift = 0;
        ulint leftChild = elementIndex * 2 + 1;
        ulint rightChild = elementIndex * 2 + 2;

        void **current = begin + elementIndex;
        void **left = begin + (elementIndex * 2 + 1);
        void **right = begin + (elementIndex * 2 + 2);

        if (right > end)
        {
            *current = valueToSift;
            return;
        }

        if (right == end)
        {
            if (Comparator (*left, valueToSift) < 0)
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
            void **better = NULL;
            ulint betterIndex;

            if (Comparator (*right, *left) < 0)
            {
                better = right;
                betterIndex = rightChild;
            }
            else
            {
                better = left;
                betterIndex = leftChild;
            }

            if (Comparator (*better, valueToSift) < 0)
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

void PArrayHeapSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second))
{
    ulint size = end - begin;
    for (ulint index = size / 2; index < size; --index)
    {
        PArrayHeapSort_SiftDown (begin, end, index, Comparator);
    }

    while (size > 0)
    {
        void *temp = *begin;
        *begin = begin [size - 1];
        begin [size - 1] = temp;
        --size;
        PArrayHeapSort_SiftDown (begin, begin + size, 0, Comparator);
    }
}

void PArrayMergeSortedParts (void **begin, void **middle, void **end,
        lint (*Comparator) (const void *first, const void *second))
{
    ulint leftPartSize = middle - begin;
    ulint rightPartSize = end - middle;

    void **leftPart = malloc (sizeof (void *) * leftPartSize);
    void **rightPart = malloc (sizeof (void *) * rightPartSize);

    void **iterator = begin;
    while (iterator != middle)
    {
        leftPart [iterator - begin] = *iterator;
        ++iterator;
    }

    while (iterator != end)
    {
        rightPart [iterator - middle] = *iterator;
        ++iterator;
    }

    iterator = begin;
    void **leftIterator = leftPart;
    void **rightIterator = rightPart;

    void **leftEnd = leftPart + leftPartSize;
    void **rightEnd = rightPart + rightPartSize;

    char moreLeft = leftIterator != leftEnd;
    char moreRight = rightIterator != rightEnd;

    while (moreLeft || moreRight)
    {
        void *best = NULL;
        if (!moreRight)
        {
            best = *leftIterator;
            ++leftIterator;
        }
        else if (!moreLeft)
        {
            best = *rightIterator;
            ++rightIterator;
        }
        else
        {
            void *left = *leftIterator;
            void *right = *rightIterator;

            if (Comparator (left, right) > 0)
            {
                best = left;
                ++leftIterator;
            }
            else
            {
                best = right;
                ++rightIterator;
            }
        }

        *iterator = best;
        ++iterator;
        moreLeft = leftIterator != leftEnd;
        moreRight = rightIterator != rightEnd;
    }

    free (leftPart);
    free (rightPart);
}

void PArrayInplaceMergeSortedParts (void **begin, void **middle, void **end,
        lint (*Comparator) (const void *first, const void *second))
{
    ulint leftPartSize = middle - begin;
    ulint rightPartSize = end - middle;
    
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

            if (Comparator (begin [leftIndex], middle [rightIndex]) >= 0)
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
        
        if (Comparator (begin [leftIndex], middle [rightIndex]) > 0)
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

    void **leftBegin = begin + (leftPartSize - mid);
    void **rightEnd = end + mid;

    void **leftIterator = leftBegin;
    void **rightIterator = middle;

    while (leftIterator != middle)
    {
        void *temp = *leftIterator;
        *leftIterator = *rightIterator;
        *rightIterator = temp;

        ++leftIterator;
        ++rightIterator;
    }

    if (rotationSize / 2 != leftPartSize)
    {
        PArrayInplaceMergeSortedParts (begin, leftBegin, middle, Comparator);
    }

    if (rotationSize / 2 != rightPartSize)
    {
        PArrayInplaceMergeSortedParts (middle, rightEnd, end, Comparator);
    }
}

void PArrayMergeSortInternal (void **begin, void **end, lint (*Comparator) (const void *first, const void *second),
        void (*Merge) (void **begin, void **middle, void **end, 
                lint (*Comparator) (const void *first, const void *second)))
{
    ulint size = end - begin;
    for (ulint subArraySize = 1; subArraySize < size; subArraySize *= 2)
    {
        void **partitionBegin = begin;
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
            void **partitionMiddle = partitionBegin + leftSize;
            void **partitionEnd = partitionMiddle + rightSize;

            PArrayMergeSortedParts (partitionBegin, partitionMiddle, partitionEnd, Comparator);
            partitionBegin = partitionEnd;
        }
    }
}

void PArrayMergeSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second))
{
    PArrayMergeSortInternal (begin, end, Comparator, PArrayMergeSortedParts);
}

void PArrayInplaceMergeSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second))
{
    PArrayMergeSortInternal (begin, end, Comparator, PArrayInplaceMergeSortedParts);
}

void **PArrayQuickSortPartition (void **begin, void **end, lint (*Comparator) (const void *first, const void *second))
{
    void **pivotIterator = end - 1;
    void *pivot = *pivotIterator;
    void **nextSmallerIterator = begin;
    void **iterator = begin;

    while (iterator != pivotIterator)
    {
        void *value = *iterator;
        if (Comparator (value, pivot) >= 0)
        {
            *iterator = *nextSmallerIterator;
            *nextSmallerIterator = value;
            ++nextSmallerIterator;
        }

        ++iterator;
    }

    *pivotIterator = *nextSmallerIterator;
    *nextSmallerIterator = pivot;
    return nextSmallerIterator;
}

void PArrayQuickSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second))
{
    ulint size = end - begin;
    void ***stack = malloc (sizeof (ulint) * size * 2);

    ulint stackSize = 0;
    stack [stackSize++] = begin;
    stack [stackSize++] = end;

    while (stackSize > 0)
    {
        end = stack [--stackSize];
        begin = stack [--stackSize];
        void **middle = PArrayQuickSortPartition (begin, end, Comparator);

        if (begin < middle)
        {
            stack [stackSize++] = begin;
            stack [stackSize++] = middle;
        }

        if (middle + 1 < end)
        {
            stack [stackSize++] = middle + 1;
            stack [stackSize++] = end;
        }
    }
}

void PArrayIntroSortInternal (void **begin, void **end, ulint depthLimit,
        lint (*Comparator) (const void *first, const void *second))
{
    if (depthLimit == 0)
    {
        PArrayHeapSort (begin, end, Comparator);
    }
    else
    {
        void **middle = PArrayQuickSortPartition (begin, end, Comparator);
        if (begin != middle)
        {
            PArrayIntroSortInternal (begin, middle, depthLimit - 1, Comparator);
        }

        if (middle + 1 != end)
        {
            PArrayIntroSortInternal (middle + 1, end, depthLimit - 1, Comparator);
        }
    }
}

void PArrayIntroSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second))
{
    ulint size = end - begin;
    ulint depthLimit = (ulint) log (size);
    PArrayIntroSortInternal (begin, end, depthLimit, Comparator);
}
