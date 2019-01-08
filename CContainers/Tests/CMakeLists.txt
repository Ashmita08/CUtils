add_executable (CContainersTests Main.c TestPVector.c TestPVector.h)
target_link_libraries (CContainersTests cunit CContainers)
add_test (NAME CContainersTests COMMAND CContainersTests)
