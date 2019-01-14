#ifndef __CUTILS_CCONTAINERS_PDOUBLELINKEDLIST_H__
#define __CUTILS_CCONTAINERS_PDOUBLELINKEDLIST_H__

#include <ShortTypes.h>

typedef void *PDoubleLinkedListHandle;
typedef void *PDoubleLinkedListIterator;

PDoubleLinkedListHandle PDoubleLinkedList_Create ();
void PDoubleLinkedList_Destruct (PDoubleLinkedListHandle handle, void (*DestructCallback) (void **item));
uint PDoubleLinkedList_Size (PDoubleLinkedListHandle handle);

PDoubleLinkedListIterator PDoubleLinkedList_Begin (PDoubleLinkedListHandle handle);
PDoubleLinkedListIterator PDoubleLinkedList_End (PDoubleLinkedListHandle handle);
PDoubleLinkedListIterator PDoubleLinkedList_At (PDoubleLinkedListHandle handle, uint index);

PDoubleLinkedListIterator PDoubleLinkedList_Insert (PDoubleLinkedListHandle handle,
        PDoubleLinkedListIterator where, void *value);
PDoubleLinkedListIterator PDoubleLinkedList_Erase (PDoubleLinkedListHandle handle,
        PDoubleLinkedListIterator iterator);

PDoubleLinkedListIterator PDoubleLinkedListIterator_Next (PDoubleLinkedListIterator iterator);
PDoubleLinkedListIterator PDoubleLinkedListIterator_Previous (PDoubleLinkedListIterator iterator);
void **PDoubleLinkedListIterator_ValueAt (PDoubleLinkedListIterator iterator);

// TODO: ForEach are equal for all iterable containers, move to CAlgorithm?
void PDoubleLinkedListIterator_ForEach (PDoubleLinkedListIterator begin, PDoubleLinkedListIterator end,
        void (*Callback) (void **item));
// TODO: ForEachReversed are equal for all 2-direction iterable containers, move to CAlgorithm?
void PDoubleLinkedListIterator_ForEachReversed (PDoubleLinkedListIterator begin, PDoubleLinkedListIterator last,
        void (*Callback) (void **item));

#endif // __CUTILS_CCONTAINERS_PDOUBLELINKEDLIST_H__
