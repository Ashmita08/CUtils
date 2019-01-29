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

void PArrayMergeSort (void **begin, void **end, lint (*Comparator) (const void *first, const void *second))
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
