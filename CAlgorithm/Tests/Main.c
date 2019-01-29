#include "TestSort.h"
#include "TestPArraySort.h"

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

int main (int argc, char **argv)
{
    RegisterSortSuite ();
    RegisterPArraySortSuite ();
    return CU_CI_RUN_SUITES ();
}
