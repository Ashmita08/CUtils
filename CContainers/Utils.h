#ifndef __CUTILS_CCONTAINERS_UTILS_H__
#define __CUTILS_CCONTAINERS_UTILS_H__

#include <ShortTypes.h>
#include "Interfaces.h"

uint CContainers_GetLastError ();
void CContainers_SetLastError (uint lastError);
void ContainerCallback_Free (void **item);

void CContainers_ForEach (VirtualHandle begin, VirtualHandle end,
        IOneDirectionIterator *IIterator, void (*Callback) (void **item));

void CContainers_ForEachReversed (VirtualHandle begin, VirtualHandle last,
        IBiDirectionalIterator *IIterator, void (*Callback) (void **item));

#endif // _CUTILS_CCONTAINERS_UTILS_H__
