add_executable (CContainersTests Main.c TestPVector.c TestPVector.h TestPSingleLinkedList.c TestPSingleLinkedList.h TestPDoubleLinkedList.c TestPDoubleLinkedList.h TestPVectorHeap.c TestPVectorHeap.h TestPHashMap.c TestPHashMap.h)
target_link_libraries (CContainersTests cunit CContainers)
add_test (NAME CContainersTests COMMAND CContainersTests)
