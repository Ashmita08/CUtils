#include "PODMemory_vs_memoryh.hpp"
#include "PVector_vs_stdvector.hpp"
#include "PHashMap_vs_stdunorderedmap.hpp"
#include "PHeap_vs_stdpriorityqueue.hpp"
#include "PVectorHeap_vs_stdpriorityqueue.hpp"

int main ()
{
    PODMemory_vs_memoryh ();
    PVector_vs_stdvector ();
    PHashMap_vs_stdunorderedmap ();
    PHeap_vs_stdpriorityqueue ();
    PVectorHeap_vs_stdpriorityqueue ();
}
