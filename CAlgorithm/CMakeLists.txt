add_library (CAlgorithm Sort.c Sort.h PArraySort.c PArraySort.h)
target_link_libraries (CAlgorithm CContainers m)
add_subdirectory (Tests)
add_subdirectory (SpeedCheck)
