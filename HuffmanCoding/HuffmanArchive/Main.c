#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <HuffmanCoding/BinaryHuffmanTree.h>

// 16 MBytes.
#define PARTITION_SIZE 16777216
#define ERROR_INCORRECT_INPUT_FILE 1
#define ERROR_INCORRECT_OUTPUT_FILE 2
#define ERROR_UNABLE_TO_WRITE_DATA 3
#define ERROR_ENCODED_FILE_BROKEN 4

static const char *Help =
    "HuffmanArchive help.\n"
    "# Usage\n"
    "    HuffmanArchive [mode] [input file] [output file]\n"
    "# Modes\n"
    "    encode\n"
    "        Encodes input file using binary huffman tree.\n"
    "    decode\n"
    "        Decodes input file using binary huffman tree.\n"
    "        Input file must contain tree header.";

static const char *EncodeModeName = "encode";
static const char *DecodeModeName = "decode";

int ShowHelp ()
{
    printf ("%s", Help);
    return 0;
}

int EncodeMode (char *inputPath, char *outputPath)
{
    int returnCode = 0;
    ullint totalBytesRead = 0;
    byte *partition;
    FILE *inputFile = fopen (inputPath, "rb");
    FILE *outputFile = fopen (outputPath, "wb");

    if (inputFile == NULL)
    {
        printf ("Input file does not exist!\n");
        return ERROR_INCORRECT_INPUT_FILE;
    }

    if (outputFile == NULL)
    {
        printf ("Unable to create output file!\n");
        return ERROR_INCORRECT_OUTPUT_FILE;
    }

    partition = malloc (PARTITION_SIZE);
    while (!feof (inputFile))
    {
        if (fgetc (inputFile) == EOF)
        {
            break;
        }
        else
        {
            fseek (inputFile, -1, SEEK_CUR);
        }

        ulint encodedSize;
        ByteCode *codes = calloc (256, sizeof (ByteCode));
        ulint partitionSize = fread (partition, 1, PARTITION_SIZE, inputFile);
        byte *encoded = Huffman_Encode (partition, partitionSize, &encodedSize, codes);

        if (fwrite (&encodedSize, sizeof (ulint), 1, outputFile) != 1)
        {
            printf ("Unable to write encoded partition size!\n");
            returnCode = ERROR_UNABLE_TO_WRITE_DATA;

            free (encoded);
            free (codes);
            break;
        }

        if (fwrite (&partitionSize, sizeof (ulint), 1, outputFile) != 1)
        {
            printf ("Unable to write partition size!\n");
            returnCode = ERROR_UNABLE_TO_WRITE_DATA;

            free (encoded);
            free (codes);
            break;
        }

        if (fwrite (codes, sizeof (ByteCode), 256, outputFile) != 256)
        {
            printf ("Unable to write Huffman tree header!\n");
            returnCode = ERROR_UNABLE_TO_WRITE_DATA;

            free (encoded);
            free (codes);
            break;
        }

        if (fwrite (encoded, sizeof (byte), encodedSize, outputFile) != encodedSize)
        {
            printf ("Unable to write encoded partition!\n");
            returnCode = ERROR_UNABLE_TO_WRITE_DATA;

            free (encoded);
            free (codes);
            break;
        }

        free (encoded);
        free (codes);

        totalBytesRead += partitionSize;
        printf ("Partition encoded! Total encoded: %dMB.\n", (int) (totalBytesRead / (1024 * 1024)));
        fflush (stdout);
    }

    fclose (inputFile);
    fclose (outputFile);
    free (partition);
    return returnCode;
}

int DecodeMode (char *inputPath, char *outputPath)
{
    int returnCode = 0;
    ullint totalBytesRead = 0;
    FILE *inputFile = fopen (inputPath, "rb");
    FILE *outputFile = fopen (outputPath, "wb");

    ulint encodedSize;
    ulint partitionSize;
    ByteCode *codes;

    if (inputFile == NULL)
    {
        printf ("Input file does not exist!\n");
        return ERROR_INCORRECT_INPUT_FILE;
    }

    if (outputFile == NULL)
    {
        printf ("Unable to create output file!\n");
        return ERROR_INCORRECT_OUTPUT_FILE;
    }

    codes = calloc (256, sizeof (ByteCode));
    while (!feof (inputFile))
    {
        if (fgetc (inputFile) == EOF)
        {
            break;
        }
        else
        {
            fseek (inputFile, -1, SEEK_CUR);
        }

        byte *encoded;
        byte *decoded;
        ulint maxDecodedSize;

        if (fread (&encodedSize, sizeof (ulint), 1, inputFile) != 1)
        {
            printf ("Unable to read encoded partition size!\n");
            returnCode = ERROR_ENCODED_FILE_BROKEN;
            break;
        }

        if (fread (&partitionSize, sizeof (ulint), 1, inputFile) != 1)
        {
            printf ("Unable to read partition size!\n");
            returnCode = ERROR_ENCODED_FILE_BROKEN;
            break;
        }

        if (fread (codes, sizeof (ByteCode), 256, inputFile) != 256)
        {
            printf ("Unable to read Huffman tree header!\n");
            returnCode = ERROR_ENCODED_FILE_BROKEN;
            break;
        }

        encoded = malloc (encodedSize);
        if (fread (encoded, sizeof (byte), encodedSize, inputFile) != encodedSize)
        {
            printf ("Unable to read encoded partition!\n");
            returnCode = ERROR_ENCODED_FILE_BROKEN;
            free (encoded);
            break;
        }

        maxDecodedSize = partitionSize;
        decoded = Huffman_Decode (encoded, encodedSize, &maxDecodedSize, codes);

        if (maxDecodedSize != partitionSize)
        {
            printf ("Decoded size is lesser than expected!\n");
            returnCode = ERROR_ENCODED_FILE_BROKEN;

            free (encoded);
            free (decoded);
            break;
        }

        if (fwrite (decoded, sizeof (byte), partitionSize, outputFile) != partitionSize)
        {
            printf ("Unable to write decoded partition!\n");
            returnCode = ERROR_UNABLE_TO_WRITE_DATA;

            free (encoded);
            free (decoded);
            break;
        }

        free (encoded);
        free (decoded);

        totalBytesRead += encodedSize + sizeof (ulint) * 2 + sizeof (ByteCode) * 256;
        printf ("Partition decoded! Total decoded: %dMB.\n", (int) (totalBytesRead / (1024 * 1024)));
        fflush (stdout);
    }

    fclose (inputFile);
    fclose (outputFile);
    free (codes);
    return returnCode;
}

int main (int args_count, char **args)
{
    if (args_count != 4)
    {
        return ShowHelp ();
    }
    else if (strcmp (EncodeModeName, args [1]) == 0)
    {
        return EncodeMode (args [2], args [3]);
    }
    else if (strcmp (DecodeModeName, args [1]) == 0)
    {
        return DecodeMode (args [2], args [3]);
    }
    else
    {
        return ShowHelp ();
    }
}
