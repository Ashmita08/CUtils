add_library (CContainers PVector.h PVector.c Utils.c Utils.h Errors.h PSingleLinkedList.c PSingleLinkedList.h
        PDoubleLinkedList.c PDoubleLinkedList.h PVectorHeap.c PVectorHeap.h PHashMap.c PHashMap.h Interfaces.h)
add_subdirectory (Tests)
add_subdirectory (SpeedCheck)
