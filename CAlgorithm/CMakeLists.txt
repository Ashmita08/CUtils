add_library (CAlgorithm Sort.c Sort.h PODArraySort.c PODArraySort.h)
target_link_libraries (CAlgorithm CContainers m)
add_subdirectory (Tests)
add_subdirectory (SpeedCheck)
