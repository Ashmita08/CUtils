#include "PSingleLinkedList.h"
#include "Utils.h"
#include "Errors.h"
#include <stdlib.h>

typedef struct
{
    void *value;
    void *next;
} PSingleLinkedListNode;

typedef struct
{
    uint size;
    PSingleLinkedListNode *head;
} PSingleLinkedList;

PSingleLinkedListHandle PSingleLinkedList_Create ()
{
    PSingleLinkedList *list = malloc (sizeof (PSingleLinkedList));
    list->size = 0;
    list->head = NULL;
    return (PSingleLinkedListHandle) list;
}

void PSingleLinkedList_Destruct (PSingleLinkedListHandle handle, void (*DestructCallback) (void **item))
{
    PSingleLinkedList *list = (PSingleLinkedList *) handle;
    PSingleLinkedListIterator iterator = PSingleLinkedList_Begin (handle);

    while (iterator != PSingleLinkedList_End (handle))
    {
        PSingleLinkedListNode *node = (PSingleLinkedListNode *) iterator;
        DestructCallback (&node->value);
        iterator = PSingleLinkedListIterator_Next (iterator);
        free (node);
    }

    free (list);
}

uint PSingleLinkedList_Size (PSingleLinkedListHandle handle)
{
    PSingleLinkedList *list = (PSingleLinkedList *) handle;
    return list->size;
}

PSingleLinkedListIterator PSingleLinkedList_Begin (PSingleLinkedListHandle handle)
{
    PSingleLinkedList *list = (PSingleLinkedList *) handle;
    return list->head;
}

PSingleLinkedListIterator PSingleLinkedList_End (PSingleLinkedListHandle handle)
{
    return NULL;
}

void PSingleLinkedList_InsertFront (PSingleLinkedListHandle handle, void *value)
{
    PSingleLinkedList *list = (PSingleLinkedList *) handle;
    list->size += 1;

    PSingleLinkedListNode *new = malloc (sizeof (PSingleLinkedListNode));
    new->value = value;
    new->next = list->head != NULL ? list->head : NULL;
    list->head = new;
}

PSingleLinkedListIterator PSingleLinkedList_At (PSingleLinkedListHandle handle, uint index)
{
    PSingleLinkedList *list = (PSingleLinkedList *) handle;
    PSingleLinkedListIterator iterator = PSingleLinkedList_Begin (handle);

    if (index >= list->size)
    {
        return NULL;
    }

    while (index--)
    {
        iterator = PSingleLinkedListIterator_Next (iterator);
    }

    return iterator;
}

PSingleLinkedListIterator PSingleLinkedList_InsertAfter (PSingleLinkedListHandle handle,
        PSingleLinkedListIterator where, void *value)
{
    PSingleLinkedList *list = (PSingleLinkedList *) handle;
    list->size += 1;

    if (list->head == NULL)
    {
        PSingleLinkedListNode *new = malloc (sizeof (PSingleLinkedListNode));
        new->value = value;
        new->next = NULL;
        list->head = new;
        return (PSingleLinkedListIterator) new;
    }

    if (where == PSingleLinkedList_End (handle))
    {
        CContainers_SetLastError (PSINGLELINKEDLIST_ERROR_INCORRECT_ITERATOR);
        return NULL;
    }

    PSingleLinkedListNode *previousNode = (PSingleLinkedListNode *) where;
    PSingleLinkedListNode *next = (PSingleLinkedListNode *) previousNode->next;
    PSingleLinkedListNode *new = malloc (sizeof (PSingleLinkedListNode));

    new->value = value;
    new->next = next;
    previousNode->next = new;
    return (PSingleLinkedListIterator) new;
}

PSingleLinkedListIterator PSingleLinkedList_Erase (PSingleLinkedListHandle handle,
        PSingleLinkedListIterator iterator)
{
    PSingleLinkedList *list = (PSingleLinkedList *) handle;
    PSingleLinkedListIterator previous = NULL;
    PSingleLinkedListIterator current = PSingleLinkedList_Begin (handle);

    while (current != PSingleLinkedList_End (handle) && current != iterator)
    {
        previous = current;
        current = PSingleLinkedListIterator_Next (current);
    }

    if (current == iterator)
    {
        list->size -= 1;
        PSingleLinkedListNode *node = (PSingleLinkedListNode *) current;

        if (list->head == node)
        {
            list->head = node->next;
        }
        else
        {
            PSingleLinkedListNode *previousNode = (PSingleLinkedListNode *) previous;
            previousNode->next = node->next;
        }

        free (node);
    }
}

PSingleLinkedListIterator PSingleLinkedList_EraseNext (PSingleLinkedListHandle handle,
        PSingleLinkedListIterator iterator)
{
    PSingleLinkedList *list = (PSingleLinkedList *) handle;
    PSingleLinkedListNode *previousNode = (PSingleLinkedListNode *) iterator;
    PSingleLinkedListNode *nodeToDelete = (PSingleLinkedListNode *) previousNode->next;

    if (nodeToDelete != NULL)
    {
        list->size -= 1;
        previousNode->next = nodeToDelete->next;
        free (nodeToDelete);
    }

    return previousNode->next;
}

PSingleLinkedListIterator PSingleLinkedListIterator_Next (PSingleLinkedListIterator iterator)
{
    PSingleLinkedListNode *node = (PSingleLinkedListNode *) iterator;
    return node->next;
}

void **PSingleLinkedListIterator_ValueAt (PSingleLinkedListIterator iterator)
{
    PSingleLinkedListNode *node = (PSingleLinkedListNode *) iterator;
    return &(node->value);
}

void PSingleLinkedListIterator_ForEach (PSingleLinkedListIterator begin, PSingleLinkedListIterator end,
        void (*Callback) (void **item))
{
    while (begin != end)
    {
        PSingleLinkedListNode *node = (PSingleLinkedListNode *) begin;
        Callback (&(node->value));
        begin = PSingleLinkedListIterator_Next (begin);
    }
}
