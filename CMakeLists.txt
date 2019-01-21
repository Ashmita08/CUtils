cmake_minimum_required (VERSION 3.0)
project (CUtils C)
enable_testing ()

set (CMAKE_C_STANDARD 11)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CUNIT_SDK}/share/cmake/cunit")

find_package (CUnit REQUIRED)
find_package (CUnitHeaders MODULE REQUIRED)

include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${CUNIT_SDK}/include)
add_subdirectory (CContainers)
