#include "TestPVector.h"

#include <cunit/CUnit.h>
#include <cunit/CUnitCI.h>

int main (int argc, char **argv)
{
    RegisterPVectorSuite ();
    return CU_CI_RUN_SUITES();
}
