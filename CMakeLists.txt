cmake_minimum_required (VERSION 3.0)
project (CUtils C CXX)
enable_testing ()

set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 11)

set (CUnit_DIR "${CUNIT_SDK}/CUnit/share/cmake/")
find_package (CUnit REQUIRED)

include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${CUNIT_SDK}/include)
add_subdirectory (CContainers)
