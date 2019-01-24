#include "Utils.h"
#include <stdlib.h>

uint LastError;

uint CContainers_GetLastError ()
{
    return LastError;
}

void CContainers_SetLastError (uint lastError)
{
    LastError = lastError;
}

void ContainerCallback_Free (void **item)
{
    free (*item);
}

void CContainers_ForEach (VirtualHandle begin, VirtualHandle end,
        IOneDirectionIterator *IIterator, void (*Callback) (void **item))
{
    while (begin != end)
    {
        Callback (IIterator->Value (begin));
        begin = IIterator->Next (begin);
    }
}

void CContainers_ForEachReversed (VirtualHandle begin, VirtualHandle last,
        IBiDirectionalIterator *IIterator, void (*Callback) (void **item))
{
    while (begin != last)
    {
        Callback (IIterator->Value (last));
        last = IIterator->Previous (last);
    }

    Callback (IIterator->Value (last));
}