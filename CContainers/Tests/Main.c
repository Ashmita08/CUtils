#include "TestPVector.h"
#include "TestPSingleLinkedList.h"
#include "TestPDoubleLinkedList.h"
#include "TestPHeap.h"
#include "TestPHashMap.h"

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

int main (int argc, char **argv)
{
    RegisterPVectorSuite ();
    RegisterPSingleLinkedListSuite ();
    RegisterPDoubleLinkedListSuite ();
    RegisterPHeapSuite ();
    RegisterPHashMapSuite ();
    return CU_CI_RUN_SUITES ();
}
