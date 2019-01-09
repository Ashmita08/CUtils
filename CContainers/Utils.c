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
