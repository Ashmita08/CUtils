add_executable (CContainersTests Main.c TestPVector.c TestPVector.h TestPSingleLinkedList.c TestPSingleLinkedList.h TestPDoubleLinkedList.c TestPDoubleLinkedList.h TestPHeap.c TestPHeap.h TestPVectorHeap.c TestPVectorHeap.h TestPHashMap.c TestPHashMap.h Utils.c Utils.h TestPODVector.c TestPODVector.h)
target_link_libraries (CContainersTests cunit CContainers)
add_test (NAME CContainersTests COMMAND CContainersTests)
