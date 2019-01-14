#include "PDoubleLinkedList.h"
#include "Utils.h"
#include "Errors.h"
#include <stdlib.h>

typedef struct
{
    void *value;
    void *next;
    void *previous;
} PDoubleLinkedListNode;

typedef struct
{
    uint size;
    PDoubleLinkedListNode *head;
    PDoubleLinkedListNode *tail;
} PDoubleLinkedList;

PDoubleLinkedListHandle PDoubleLinkedList_Create ()
{
    PDoubleLinkedList *list = malloc (sizeof (PDoubleLinkedList));
    list->size = 0;

    PDoubleLinkedListNode *endMarker = malloc (sizeof (PDoubleLinkedListNode));
    endMarker->value = NULL;
    endMarker->next = NULL;
    endMarker->previous = NULL;

    list->head = endMarker;
    list->tail = endMarker;
    return (PDoubleLinkedListHandle) list;
}

void PDoubleLinkedList_Destruct (PDoubleLinkedListHandle handle, void (*DestructCallback) (void **item))
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    PDoubleLinkedListNode *node = list->head;

    while (node != NULL)
    {
        PDoubleLinkedListNode *next = (PDoubleLinkedListNode *) node->next;
        DestructCallback (&(node->value));
        free (node);
        node = next;
    }

    free (list);
}

uint PDoubleLinkedList_Size (PDoubleLinkedListHandle handle)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    return list->size;
}

PDoubleLinkedListIterator PDoubleLinkedList_Begin (PDoubleLinkedListHandle handle)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    return list->head;
}

PDoubleLinkedListIterator PDoubleLinkedList_End (PDoubleLinkedListHandle handle)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    return list->tail;
}

PDoubleLinkedListIterator PDoubleLinkedList_At (PDoubleLinkedListHandle handle, uint index)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    if (index >= list->size)
    {
        return NULL;
    }

    PDoubleLinkedListIterator iterator = PDoubleLinkedList_Begin (handle);
    while (index--)
    {
        iterator = PDoubleLinkedListIterator_Next (iterator);
    }

    return iterator;
}

PDoubleLinkedListIterator PDoubleLinkedList_Insert (PDoubleLinkedListHandle handle,
        PDoubleLinkedListIterator where, void *value)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    list->size += 1;

    PDoubleLinkedListNode *next = (PDoubleLinkedListNode *) where;
    PDoubleLinkedListNode *previous = next->previous;

    PDoubleLinkedListNode *new = malloc (sizeof (PDoubleLinkedListNode));
    new->value = value;
    new->next = next;
    new->previous = previous;

    next->previous = new;
    if (previous != NULL)
    {
        previous->next = new;
    }
    else
    {
        list->head = new;
    }

    return (PDoubleLinkedListIterator) new;
}

PDoubleLinkedListIterator PDoubleLinkedList_Erase (PDoubleLinkedListHandle handle,
        PDoubleLinkedListIterator iterator)
{
    if (iterator == PDoubleLinkedList_End (handle))
    {
        CContainers_SetLastError (PDOUBLELINKEDLIST_ERROR_INCORRECT_ITERATOR);
        return NULL;
    }

    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    list->size -= 1;

    PDoubleLinkedListNode *delete = (PDoubleLinkedListNode *) iterator;
    PDoubleLinkedListNode *next = delete->next;
    PDoubleLinkedListNode *previous = delete->previous;

    next->previous = previous;
    if (previous != NULL)
    {
        previous->next = next;
    }
    else
    {
        list->head = next;
    }

    free (delete);
    return (PDoubleLinkedListIterator) next;
}

PDoubleLinkedListIterator PDoubleLinkedListIterator_Next (PDoubleLinkedListIterator iterator)
{
    PDoubleLinkedListNode *node = (PDoubleLinkedListNode *) iterator;
    return (PDoubleLinkedListIterator) node->next;
}

PDoubleLinkedListIterator PDoubleLinkedListIterator_Previous (PDoubleLinkedListIterator iterator)
{
    PDoubleLinkedListNode *node = (PDoubleLinkedListNode *) iterator;
    return (PDoubleLinkedListIterator) node->previous;
}

void **PDoubleLinkedListIterator_ValueAt (PDoubleLinkedListIterator iterator)
{
    PDoubleLinkedListNode *node = (PDoubleLinkedListNode *) iterator;
    return &(node->value);
}

void PDoubleLinkedListIterator_ForEach (PDoubleLinkedListIterator begin, PDoubleLinkedListIterator end,
        void (*Callback) (void **item))
{
    while (begin != end)
    {
        Callback (PDoubleLinkedListIterator_ValueAt (begin));
        begin = PDoubleLinkedListIterator_Next (begin);
    }
}

void PDoubleLinkedListIterator_ForEachReversed (PDoubleLinkedListIterator begin, PDoubleLinkedListIterator last,
        void (*Callback) (void **item))
{
    do
    {
        Callback (PDoubleLinkedListIterator_ValueAt (last));
        last = PDoubleLinkedListIterator_Previous (last);
    } while (begin != last);
}
