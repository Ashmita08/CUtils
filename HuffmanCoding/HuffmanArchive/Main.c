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
            return ERROR_UNABLE_TO_WRITE_DATA;
        }

        if (fwrite (&partitionSize, sizeof (ulint), 1, outputFile) != 1)
        {
            printf ("Unable to write partition size!\n");
            return ERROR_UNABLE_TO_WRITE_DATA;
        }

        if (fwrite (codes, sizeof (ByteCode), 256, outputFile) != 256)
        {
            printf ("Unable to write Huffman tree header!\n");
            return ERROR_UNABLE_TO_WRITE_DATA;
        }

        if (fwrite (encoded, sizeof (byte), encodedSize, outputFile) != encodedSize)
        {
            printf ("Unable to write encoded partition!\n");
            return ERROR_UNABLE_TO_WRITE_DATA;
        }

        fflush (outputFile);
        free (encoded);
        free (codes);
    }

    fclose (inputFile);
    fclose (outputFile);
    free (partition);
    return 0;
}

int DecodeMode (char *inputPath, char *outputPath)
{
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
            return ERROR_ENCODED_FILE_BROKEN;
        }

        if (fread (&partitionSize, sizeof (ulint), 1, inputFile) != 1)
        {
            printf ("Unable to read partition size!\n");
            return ERROR_ENCODED_FILE_BROKEN;
        }

        if (fread (codes, sizeof (ByteCode), 256, inputFile) != 256)
        {
            printf ("Unable to read Huffman tree header!\n");
            return ERROR_ENCODED_FILE_BROKEN;
        }

        encoded = malloc (encodedSize);
        ulint tmp;
        if ((tmp = fread (encoded, sizeof (byte), encodedSize, inputFile)) != encodedSize)
        {
            printf ("Unable to read encoded partition!\n");
            return ERROR_ENCODED_FILE_BROKEN;
        }

        maxDecodedSize = partitionSize;
        decoded = Huffman_Decode (encoded, encodedSize, &maxDecodedSize, codes);

        if (maxDecodedSize != partitionSize)
        {
            printf ("Decoded size is lesser than expected!\n");
            return ERROR_ENCODED_FILE_BROKEN;
        }

        if (fwrite (decoded, sizeof (byte), partitionSize, outputFile) != partitionSize)
        {
            printf ("Unable to write decoded partition!\n");
            return ERROR_UNABLE_TO_WRITE_DATA;
        }

        fflush (outputFile);
        free (encoded);
        free (decoded);
    }

    fclose (inputFile);
    fclose (outputFile);
    free (codes);
    return 0;
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
