#include "Sort.h"
#include <CContainers/PVector.h>
#include <CContainers/PVectorHeap.h>
#include <CContainers/Utils.h>

void HeapSort (VirtualHandle container, ISizedContainer *ISized,
        IIterableContainer *IIterable, IOneDirectionIterator *IIterator,
        lint (*Comparator) (const void *first, const void *second))
{
    PVectorHandle vector = PVector_Create (ISized->Size (container));
    VirtualHandle iterator = IIterable->Begin (container);

    while (iterator != IIterable->End (container))
    {
        PVector_Insert (vector, PVector_End (vector), *(IIterator->Value (iterator)));
        iterator = IIterator->Next (iterator);
    }

    PVectorHeapHandle heap = PVectorHeap_Heapify (vector, Comparator);
    iterator = IIterable->Begin (container);

    while (iterator != IIterable->End (container))
    {
        // Const qualifier discarded not by mistake!
        *(IIterator->Value (iterator)) = PVectorHeap_Top (heap);
        PVectorHeap_Pop (heap);
        iterator = IIterator->Next (iterator);
    }

    PVectorHeap_Destruct (heap, ContainerCallback_NoAction);
}
