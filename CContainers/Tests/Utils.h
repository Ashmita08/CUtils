#ifndef __CUTILS_CCONTAINERS_TESTS_UTILS_H__
#define __CUTILS_CCONTAINERS_TESTS_UTILS_H__

#include <ShortTypes.h>
#include <CContainers/Interfaces.h>

void SizeChecker (VirtualHandle container, ISizedContainer *ISized, ulint expected, char fatal);
void IterationNaturalOrderChecker (VirtualHandle container, IIterableContainer *IIterable,
        IOneDirectionIterator *IIterator, char fatal);

void IterationReversedNaturalOrderChecker (VirtualHandle container,
        ISizedContainer *ISized, IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, char fatal);
void ItemAtNaturalOrderChecker (VirtualHandle container, ISizedContainer *ISized, IIterableContainer *IIterable,
        IOneDirectionIterator *IIterator, char fatal);

#endif // _CUTILS_CCONTAINERS_TESTS_UTILS_H__
