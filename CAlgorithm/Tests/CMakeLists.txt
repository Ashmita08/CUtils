add_executable (CAlgorithmTests Main.c TestSort.c TestSort.h TestPODArraySort.c TestPODArraySort.h)
target_link_libraries (CAlgorithmTests CAlgorithm cunit)
add_test (NAME CAlgorithmTests COMMAND CAlgorithmTests)
