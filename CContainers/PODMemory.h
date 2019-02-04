#ifndef __CUTILS_CCONTAINERS_PODMEMORY_H__
#define __CUTILS_CCONTAINERS_PODMEMORY_H__

#include <ShortTypes.h>

void PODMemory_Swap (char *firstElement, char *secondElement, ulint elementSize);
void PODMemory_Copy (char *destination, const char *source, ulint elementSize);
void PODMemory_MoveRight (char *memory, ulint elementSize, ulint offset);
void PODMemory_MoveLeft (char *memory, ulint elementSize, ulint offset);

#endif // __CUTILS_CCONTAINERS_PODMEMORY_H__
