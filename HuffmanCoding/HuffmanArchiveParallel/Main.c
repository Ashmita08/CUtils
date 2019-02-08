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
#define ERROR_INCORRECT_THREAD_COUNT 5

static const char *Help =
    "HuffmanArchive help.\n"
    "# Usage\n"
    "    HuffmanArchiveParallel [parallel-partitions-count] [mode] [input file] [output file]\n"
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

int EncodeMode (ulint parallelPartitionsCount, char *inputPath, char *outputPath)
{
    ullint totalBytesRead = 0;
    byte **partitions;
    ulint *partitionsSizes;
    byte **encodedPartitions;
    ByteCode **codes;
    ulint *encodedSizes;

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

    partitions = malloc (sizeof (byte *) * parallelPartitionsCount);
    for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
    {
        partitions [partitionIndex] = malloc (PARTITION_SIZE);
    }

    partitionsSizes = calloc (parallelPartitionsCount, sizeof (ulint));
    encodedPartitions = calloc (parallelPartitionsCount, sizeof (byte *));

    codes = malloc (sizeof (ByteCode *) * parallelPartitionsCount);
    for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
    {
        codes [partitionIndex] = calloc (256, sizeof (ByteCode));
    }

    encodedSizes = calloc (parallelPartitionsCount, sizeof (ulint));
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

        for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
        {
            memset (codes [partitionIndex], 0, 256 * sizeof (ByteCode));
            partitionsSizes [partitionIndex] =
                fread (partitions [partitionIndex], sizeof (byte), PARTITION_SIZE, inputFile);
        }

#pragma omp parallel for firstprivate (encodedPartitions) firstprivate (partitionsSizes) \
    firstprivate (partitions) firstprivate (encodedSizes) firstprivate (codes)
        for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
        {
            encodedPartitions [partitionIndex] = partitionsSizes [partitionIndex] > 0 ?
                Huffman_Encode (partitions [partitionIndex], partitionsSizes [partitionIndex],
                    encodedSizes + partitionIndex, codes [partitionIndex]) : NULL;
        }

        for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
        {
            if (encodedPartitions [partitionIndex] != NULL)
            {
                if (fwrite (encodedSizes + partitionIndex, sizeof (ulint), 1, outputFile) != 1)
                {
                    printf ("Unable to write encoded partition size!\n");
                    return ERROR_UNABLE_TO_WRITE_DATA;
                }

                if (fwrite (partitionsSizes + partitionIndex, sizeof (ulint), 1, outputFile) != 1)
                {
                    printf ("Unable to write partition size!\n");
                    return ERROR_UNABLE_TO_WRITE_DATA;
                }

                if (fwrite (codes [partitionIndex], sizeof (ByteCode), 256, outputFile) != 256)
                {
                    printf ("Unable to write Huffman tree header!\n");
                    return ERROR_UNABLE_TO_WRITE_DATA;
                }

                if (fwrite (encodedPartitions [partitionIndex], sizeof (byte),
                    encodedSizes [partitionIndex], outputFile) != encodedSizes [partitionIndex])
                {
                    printf ("Unable to write encoded partition!\n");
                    return ERROR_UNABLE_TO_WRITE_DATA;
                }

                free (encodedPartitions [partitionIndex]);
                totalBytesRead += partitionsSizes [partitionIndex];
            }
        }

        printf ("Parallel partitions encoded! Total encoded: %dMB.\n", (int) (totalBytesRead / (1024 * 1024)));
        fflush (stdout);
    }

    fclose (inputFile);
    fclose (outputFile);

    for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
    {
        free (partitions [partitionIndex]);
    }

    free (partitions);
    free (partitionsSizes);
    free (encodedPartitions);

    for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
    {
        free (codes [partitionIndex]);
    }

    free (codes);
    free (encodedSizes);
    return 0;
}

int DecodeMode (ulint parallelPartitionsCount, char *inputPath, char *outputPath)
{
    ullint totalBytesRead = 0;
    FILE *inputFile = fopen (inputPath, "rb");
    FILE *outputFile = fopen (outputPath, "wb");

    ulint *encodedSizes;
    ulint *partitionSizes;
    byte **encodedPartitions;
    byte **decodedPartitions;
    ByteCode **codes;

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

    encodedSizes = calloc (parallelPartitionsCount, sizeof (ulint));
    partitionSizes = calloc (parallelPartitionsCount, sizeof (ulint));
    codes = malloc (sizeof (ByteCode *) * parallelPartitionsCount);

    for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
    {
        codes [partitionIndex] = calloc (256, sizeof (ByteCode));
    }

    encodedPartitions = calloc (parallelPartitionsCount, sizeof (byte *));
    decodedPartitions = calloc (parallelPartitionsCount, sizeof (byte *));

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

        for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
        {
            if (fread (encodedSizes + partitionIndex, sizeof (ulint), 1, inputFile) == 1)
            {
                if (fread (partitionSizes + partitionIndex, sizeof (ulint), 1, inputFile) != 1)
                {
                    printf ("Unable to read partition size!\n");
                    // FIXME: Rework error returns both there and non-parallel version! Memory leaks!
                    return ERROR_ENCODED_FILE_BROKEN;
                }

                if (fread (codes [partitionIndex], sizeof (ByteCode), 256, inputFile) != 256)
                {
                    printf ("Unable to read Huffman tree header!\n");
                    return ERROR_ENCODED_FILE_BROKEN;
                }

                encodedPartitions [partitionIndex] = malloc (encodedSizes [partitionIndex]);
                if (fread (encodedPartitions [partitionIndex], sizeof (byte),
                    encodedSizes [partitionIndex], inputFile) != encodedSizes [partitionIndex])
                {
                    printf ("Unable to read encoded partition!\n");
                    return ERROR_ENCODED_FILE_BROKEN;
                }
            }
            else
            {
                encodedSizes [partitionIndex] = 0;
                partitionSizes [partitionIndex] = 0;
                encodedPartitions [partitionIndex] = NULL;
            }
        }

#pragma omp parallel for firstprivate (encodedPartitions) firstprivate (partitionSizes) \
    firstprivate (decodedPartitions) firstprivate (encodedSizes) firstprivate (codes)
        for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
        {
            if (encodedSizes [partitionIndex] > 0)
            {
                ulint maxDecodedSize = partitionSizes [partitionIndex];
                decodedPartitions [partitionIndex] = Huffman_Decode (
                    encodedPartitions [partitionIndex], encodedSizes [partitionIndex],
                    &maxDecodedSize, codes [partitionIndex]);

                free (encodedPartitions [partitionIndex]);
                if (maxDecodedSize != partitionSizes [partitionIndex])
                {
                    printf ("Decoded size is lesser than expected!\n");
                    //return ERROR_ENCODED_FILE_BROKEN;
                }
            }
        }

        for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
        {
            if (encodedSizes [partitionIndex] > 0)
            {
                if (fwrite (decodedPartitions [partitionIndex], sizeof (byte),
                    partitionSizes [partitionIndex], outputFile) != partitionSizes [partitionIndex])
                {
                    printf ("Unable to write decoded partition!\n");
                    //return ERROR_UNABLE_TO_WRITE_DATA;
                }

                free (decodedPartitions [partitionIndex]);
                totalBytesRead += encodedSizes [partitionIndex] + 256 * sizeof (ByteCode) + 2 * sizeof (ulint);
            }
        }

        printf ("Parallel partitions decoded! Total decoded: %dMB.\n", (int) (totalBytesRead / (1024 * 1024)));
        fflush (stdout);
    }

    fclose (inputFile);
    fclose (outputFile);

    for (ulint partitionIndex = 0; partitionIndex < parallelPartitionsCount; ++partitionIndex)
    {
        free (codes [partitionIndex]);
    }

    free (codes);
    free (encodedSizes);
    free (partitionSizes);
    free (encodedPartitions);
    free (decodedPartitions);
    return 0;
}

int main (int args_count, char **args)
{
    if (args_count != 5)
    {
        return ShowHelp ();
    }
    else
    {
        int parallelPartitionsCount = atoi (args [1]);
        if (parallelPartitionsCount <= 0)
        {
            printf ("Incorrect thread count!\n");
            return ERROR_INCORRECT_THREAD_COUNT;
        }

        if (strcmp (EncodeModeName, args [2]) == 0)
        {
            return EncodeMode ((ulint) parallelPartitionsCount, args [3], args [4]);
        }
        else if (strcmp (DecodeModeName, args [2]) == 0)
        {
            return DecodeMode ((ulint) parallelPartitionsCount, args [3], args [4]);
        }
        else
        {
            return ShowHelp ();
        }
    }
}
