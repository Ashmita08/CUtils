#include "Utils.h"
#include <CUnit/CUnit.h>
#include <CContainers/Utils.h>

void SizeChecker (VirtualHandle container, ISizedContainer *ISized, ulint expected, char fatal)
{
    if (expected != ISized->Size (container))
    {
        printf ("\n        Size (e/a): %d, %d.", (int) expected, (int) ISized->Size (container));
        if (fatal)
        {
            CU_FAIL_FATAL ("Expected and actual values are not equal!");
        }
        else
        {
            CU_FAIL ("Expected and actual values are not equal!");
        }
    }
}

void IterationNaturalOrderChecker (VirtualHandle container, IIterableContainer *IIterable,
        IOneDirectionIterator *IIterator, char fatal)
{
    int number = 0;
    VirtualHandle iterator = IIterable->Begin (container);

    while (iterator != IIterable->End (container))
    {
        int value = **((int **) IIterator->Value (iterator));
        if (value != number)
        {
            printf ("\n        Value at %d (e/a): %d, %d.", number, number, value);
            if (fatal)
            {
                CU_FAIL_FATAL ("Container content is incorrect!");
            }
            else
            {
                CU_FAIL ("Container content is incorrect!");
            }
        }

        iterator = IIterator->Next (iterator);
        ++number;
    }
}

void IterationReversedNaturalOrderChecker (VirtualHandle container,
        ISizedContainer *ISized, IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, char fatal)
{
    int number = (int) ISized->Size (container) - 1;
    VirtualHandle iterator = IIterator->Previous (IIterable->End (container));

    do
    {
        int value = **((int **) IIterator->Value (iterator));
        if (value != number)
        {
            printf ("\n        Value at %d (e/a): %d, %d.", number, number, value);
            if (fatal)
            {
                CU_FAIL_FATAL ("Container content is incorrect!");
            }
            else
            {
                CU_FAIL ("Container content is incorrect!");
            }
        }

        iterator = IIterator->Previous (iterator);
        --number;
    }
    while (iterator != IIterable->Begin (container));
}

void ItemAtNaturalOrderChecker (VirtualHandle container, ISizedContainer *ISized, IIterableContainer *IIterable,
        IOneDirectionIterator *IIterator, char fatal)
{
    for (uint index = 0; index < ISized->Size (container); ++index)
    {
        VirtualHandle iterator = IIterable->At (container, index);
        if (CContainers_GetLastError () != 0)
        {
            printf ("\n        CContainers internal error code: %d.\n", CContainers_GetLastError ());
            if (fatal)
            {
                CU_FAIL_FATAL ("CContainers internal error!");
            }
            else
            {
                CU_FAIL ("CContainers internal error!");
            }
        }

        int value = **((int **) IIterator->Value (iterator));
        if (value != index)
        {
            printf ("\n        Value at %d (e/a): %d, %d.", index, index, value);
            if (fatal)
            {
                CU_FAIL_FATAL ("Container content is incorrect!");
            }
            else
            {
                CU_FAIL ("Container content is incorrect!");
            }
        }
    }
}
