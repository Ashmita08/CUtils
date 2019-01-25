#include "Sort.h"
#include <CContainers/PVector.h>
#include <CContainers/PVectorHeap.h>
#include <CContainers/Utils.h>

static VirtualHandle HeapSort_DummyErase (VirtualHandle container, VirtualHandle iterator)
{

}

static void HeapSort_DummyDestruct (VirtualHandle container, void (*Callback) (void **item))
{

}

void HeapSort (VirtualHandle container, ISizedContainer *ISized,
        IIterableContainer *IIterable, IBiDirectionalIterator *IIterator, IMutableContainer *IMutable,
        lint (*Comparator) (const void *first, const void *second))
{
    IMutableContainer ICustomMutable = *IMutable;
    ICustomMutable.Erase = HeapSort_DummyErase;
    PVectorHeapHandle heap = PVectorHeap_Heapify (
            container, ISized, IIterable, IIterator, &ICustomMutable, Comparator, 1);

    VirtualHandle iterator = IIterator->Previous (IIterable->End (container));
    do
    {
        const void *value = PVectorHeap_Top (heap);
        PVectorHeap_Pop (heap);
        // Const qualifier discarded not by mistake!
        *(IIterator->Value (iterator)) = value;

        if (iterator == IIterable->Begin (container))
        {
            break;
        }

        iterator = IIterator->Previous (iterator);
    }
    while (1);

    PVectorHeap_Destruct (heap, HeapSort_DummyDestruct, ContainerCallback_NoAction);
}
