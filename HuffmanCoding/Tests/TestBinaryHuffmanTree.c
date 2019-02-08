#include "TestBinaryHuffmanTree.h"
#include <HuffmanCoding/BinaryHuffmanTree.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <stdlib.h>
#include <string.h>

static const char *FIRST_COMPRESSION_TEST = "go, go, googlers";
#define FIRST_COMPRESSION_TEST_EXPECTED_SIZE 6

static const char *SECOND_COMPRESSION_TEST = "adsfaukjha asudhfbaf wihlkqwrifsduii 128413 diha 0sds /./;}\\??..!";
#define SECOND_COMPRESSION_TEST_EXPECTED_SIZE 37

static ByteCode *codes;

static void BinaryHuffmanTreeSuite_CheckCompression (const char *string, ulint expectedResultSize)
{
    ulint codeSize;
    byte *result = Huffman_Encode ((byte *) string, strlen (string), &codeSize, codes);

    if (codeSize != expectedResultSize)
    {
        printf ("\nCompressed size (e/a): %d/%d.", (int) expectedResultSize, (int) codeSize);
        CU_FAIL ("Compressed size doesn't match calculated expectation!")
    }

    free (result);
}

static void BinaryHuffmanTreeSuite_CheckDecompression (const char *string)
{
    ulint codeSize;
    ulint maxDecodedSize = strlen (string) + 1;

    byte *encoded = Huffman_Encode ((byte *) string, maxDecodedSize, &codeSize, codes);
    byte *decoded = Huffman_Decode (encoded, codeSize, &maxDecodedSize, codes);

    if (strcmp (string, (char *) decoded) != 0)
    {
        printf ("\nSource:  \"%s\".", string);
        printf ("\nDecoded: \"%s\".", (char *) decoded);
        CU_FAIL ("Decoded message isn't equal to source!");
    }

    free (encoded);
    free (decoded);
}

static void BinaryHuffmanTreeSuite_Setup ()
{
    codes = calloc (256, sizeof (ByteCode));
}

static void BinaryHuffmanTreeSuite_Teardown ()
{
    free (codes);
}

static void BinaryHuffmanTreeSuite_FirstCompressionTest ()
{
    BinaryHuffmanTreeSuite_CheckCompression (FIRST_COMPRESSION_TEST, FIRST_COMPRESSION_TEST_EXPECTED_SIZE);
}

static void BinaryHuffmanTreeSuite_SecondCompressionTest ()
{
    BinaryHuffmanTreeSuite_CheckCompression (SECOND_COMPRESSION_TEST, SECOND_COMPRESSION_TEST_EXPECTED_SIZE);
}

static void BinaryHuffmanTreeSuite_FirstDecompressionTest ()
{
    BinaryHuffmanTreeSuite_CheckDecompression (FIRST_COMPRESSION_TEST);
}

static void BinaryHuffmanTreeSuite_SecondDecompressionTest ()
{
    BinaryHuffmanTreeSuite_CheckDecompression (SECOND_COMPRESSION_TEST);
}

void RegisterBinaryHuffmanTreeSuite ()
{
    CU_CI_DEFINE_SUITE ("HuffmanCoding", NULL, NULL, BinaryHuffmanTreeSuite_Setup, BinaryHuffmanTreeSuite_Teardown);
    CUNIT_CI_TEST (BinaryHuffmanTreeSuite_FirstCompressionTest);
    CUNIT_CI_TEST (BinaryHuffmanTreeSuite_SecondCompressionTest);

    CUNIT_CI_TEST (BinaryHuffmanTreeSuite_FirstDecompressionTest);
    CUNIT_CI_TEST (BinaryHuffmanTreeSuite_SecondDecompressionTest);
}
