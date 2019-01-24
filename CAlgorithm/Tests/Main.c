#include "TestSort.h"

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

int main (int argc, char **argv)
{
    RegisterSortSuite ();
    return CU_CI_RUN_SUITES ();
}
