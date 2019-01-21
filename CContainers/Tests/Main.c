#include "TestPVector.h"
#include "TestPSingleLinkedList.h"
#include "TestPDoubleLinkedList.h"
#include "TestPVectorHeap.h"
#include "TestPHashMap.h"

#include <cunit/CUnit.h>
#include <cunit/CUnitCI.h>

int main (int argc, char **argv)
{
    RegisterPVectorSuite ();
    RegisterPSingleLinkedListSuite ();
    RegisterPDoubleLinkedListSuite ();
    RegisterPVectorHeapSuite ();
    RegisterPHashMapSuite ();
    return CU_CI_RUN_SUITES();
}
