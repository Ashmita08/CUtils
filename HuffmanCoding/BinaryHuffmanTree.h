#ifndef __CUTILS_HUFFMANCODING_BINARYHUFFMANTREE_H__
#define __CUTILS_HUFFMANCODING_BINARYHUFFMANTREE_H__

#include <ShortTypes.h>
#define MAX_HUFFMAN_CODE_LENGTH_IN_BYTES (256 / 8) /* MaxTreeDepth / BitsInByte. */
#define output

typedef unsigned char byte;
typedef struct
{
    ushort bitLength;
    byte code [MAX_HUFFMAN_CODE_LENGTH_IN_BYTES];
} ByteCode;

/// Encodes given input using Huffman algorithm.
/// @param input [in] array to encode.
/// @param input_length [in] length of input array in bytes.
/// @param codeSize [out] result Huffman code length in bytes.
/// @param resultCodes [out] ByteCode map output, must be zero-filled array of 256 ByteCodes.
/// @return encoded bytes array.
byte *Huffman_Encode (byte *input, ulint input_length, output ulint *codeSize, output ByteCode *resultCodes);

/// Decodes given input using Huffman algorithm.
/// @param input [in] array to decode.
/// @param input_length [in] length of input array in bytes.
/// @param maxDecodedSize [in] count of bytes to decode from encoded sequence, if there are less bytes this value would be overwritten.
/// @param codes [in] ByteCode map, previously used to encode this bytes sequence.
/// @return decoded bytes array.
byte *Huffman_Decode (byte *input, ulint input_length, ulint *maxDecodedSize, ByteCode *codes);

#endif // __CUTILS_HUFFMANCODING_BINARYHUFFMANTREE_H__
