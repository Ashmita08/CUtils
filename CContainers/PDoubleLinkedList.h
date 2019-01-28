#ifndef __CUTILS_CCONTAINERS_PDOUBLELINKEDLIST_H__
#define __CUTILS_CCONTAINERS_PDOUBLELINKEDLIST_H__

#include <ShortTypes.h>
#include "Interfaces.h"

typedef void *PDoubleLinkedListHandle;
typedef void *PDoubleLinkedListIterator;

PDoubleLinkedListHandle PDoubleLinkedList_Create ();
void PDoubleLinkedList_Destruct (PDoubleLinkedListHandle handle, void (*DestructCallback) (void **item));
ulint PDoubleLinkedList_Size (PDoubleLinkedListHandle handle);

PDoubleLinkedListIterator PDoubleLinkedList_Begin (PDoubleLinkedListHandle handle);
PDoubleLinkedListIterator PDoubleLinkedList_End (PDoubleLinkedListHandle handle);
PDoubleLinkedListIterator PDoubleLinkedList_At (PDoubleLinkedListHandle handle, ulint index);

PDoubleLinkedListIterator PDoubleLinkedList_Insert (PDoubleLinkedListHandle handle,
        PDoubleLinkedListIterator where, void *value);
PDoubleLinkedListIterator PDoubleLinkedList_Erase (PDoubleLinkedListHandle handle,
        PDoubleLinkedListIterator iterator);

PDoubleLinkedListIterator PDoubleLinkedListIterator_Next (PDoubleLinkedListIterator iterator);
PDoubleLinkedListIterator PDoubleLinkedListIterator_Jump (PDoubleLinkedListIterator iterator, ulint distance);
PDoubleLinkedListIterator PDoubleLinkedListIterator_Previous (PDoubleLinkedListIterator iterator);
PDoubleLinkedListIterator PDoubleLinkedListIterator_JumpBack (PDoubleLinkedListIterator iterator, ulint distance);
void **PDoubleLinkedListIterator_ValueAt (PDoubleLinkedListIterator iterator);

IOneDirectionIterator *PDoubleLinkedListIterator_AsIOneDirectionIterator ();
IBiDirectionalIterator *PDoubleLinkedListIterator_AsIBiDirectionalIterator ();
ISizedContainer *PDoubleLinkedList_AsISizedContainer ();
IIterableContainer *PDoubleLinkedList_AsIIterableContainer ();
IMutableContainer *PDoubleLinkedList_AsIMutableContainer ();

#endif // __CUTILS_CCONTAINERS_PDOUBLELINKEDLIST_H__
