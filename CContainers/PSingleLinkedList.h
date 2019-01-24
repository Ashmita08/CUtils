#ifndef __CUTILS_CCONTAINERS_PSINGLELINKEDLIST_H__
#define __CUTILS_CCONTAINERS_PSINGLELINKEDLIST_H__

#include <ShortTypes.h>
#include "Interfaces.h"

typedef void *PSingleLinkedListHandle;
typedef void *PSingleLinkedListIterator;

PSingleLinkedListHandle PSingleLinkedList_Create ();
void PSingleLinkedList_Destruct (PSingleLinkedListHandle handle, void (*DestructCallback) (void **item));
ulint PSingleLinkedList_Size (PSingleLinkedListHandle handle);

PSingleLinkedListIterator PSingleLinkedList_Begin (PSingleLinkedListHandle handle);
PSingleLinkedListIterator PSingleLinkedList_End (PSingleLinkedListHandle handle);
PSingleLinkedListIterator PSingleLinkedList_At (PSingleLinkedListHandle handle, ulint index);

void PSingleLinkedList_InsertFront (PSingleLinkedListHandle handle, void *value);
PSingleLinkedListIterator PSingleLinkedList_InsertAfter (PSingleLinkedListHandle handle,
        PSingleLinkedListIterator where, void *value);
PSingleLinkedListIterator PSingleLinkedList_Erase (PSingleLinkedListHandle handle,
        PSingleLinkedListIterator iterator);
PSingleLinkedListIterator PSingleLinkedList_EraseNext (PSingleLinkedListHandle handle,
        PSingleLinkedListIterator iterator);

PSingleLinkedListIterator PSingleLinkedListIterator_Next (PSingleLinkedListIterator iterator);
void **PSingleLinkedListIterator_ValueAt (PSingleLinkedListIterator iterator);

IOneDirectionIterator *PSingleLinkedListIterator_AsIOneDirectionIterator ();
ISizedContainer *PSingleLinkedList_AsISizedContainer ();
IIterableContainer *PSingleLinkedList_AsIIterableContainer ();
IOrganizerContainer *PSingleLinkedList_AsIOrganizerContainer ();

#endif // __CUTILS_CCONTAINERS_PSINGLELINKEDLIST_H__
