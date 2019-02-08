add_executable (HuffmanCodingTests Main.c TestBinaryHuffmanTree.c TestBinaryHuffmanTree.h)
target_link_libraries (HuffmanCodingTests cunit HuffmanCoding)
add_test (NAME HuffmanCodingTests COMMAND HuffmanCodingTests)
