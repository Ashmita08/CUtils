#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include "TestBinaryHuffmanTree.h"

int main (int argc, char **argv)
{
    RegisterBinaryHuffmanTreeSuite ();
    return CU_CI_RUN_SUITES ();
}
