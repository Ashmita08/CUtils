#include "TestSort.h"
#include "TestPODArraySort.h"

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

int main (int argc, char **argv)
{
    RegisterSortSuite ();
    RegisterPODArraySortSuite ();
    return CU_CI_RUN_SUITES ();
}
