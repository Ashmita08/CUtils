#ifndef __CUTILS_CCONTAINERS_UTILS_H__
#define __CUTILS_CCONTAINERS_UTILS_H__

#include <ShortTypes.h>

uint CContainers_GetLastError ();
void CContainers_SetLastError (uint lastError);
void ContainerCallback_Free (void **item);

#endif // _CUTILS_CCONTAINERS_UTILS_H__
