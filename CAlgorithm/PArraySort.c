#include "PArraySort.h"
#include <stdlib.h>

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
