add_library (CContainers PVector.h PVector.c Utils.c Utils.h Errors.h PSingleLinkedList.c PSingleLinkedList.h
        PDoubleLinkedList.c PDoubleLinkedList.h PHeap.c PHeap.h PHashMap.c PHashMap.h Interfaces.h)
add_subdirectory (Tests)
add_subdirectory (SpeedCheck)
