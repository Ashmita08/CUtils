#include "BinaryHuffmanTree.h"
#include <stdlib.h>
#include <memory.h>

#include <CContainers/PODVector.h>
#include <CContainers/PVectorHeap.h>
#include <CContainers/Utils.h>

#define INITIAL_TREE_NODES_CAPACITY 257
typedef struct
{
    ushort zeroChild;
    ushort oneChild;
    byte leafValue;
} Node;

typedef struct
{
    Node super;
    ulint priority;
} NodeWithPriority;

typedef struct
{
    NodeWithPriority super;
    ushort index;
} NodeWithPriorityAndIndex;

static void SetBitInCode (byte *codeBuffer, ulint bitIndex, byte value)
{
    codeBuffer += bitIndex >> 3;
    bitIndex = (byte) (7 - bitIndex & 7);

    if (*codeBuffer & (1 << bitIndex))
    {
        *codeBuffer &= value << bitIndex;
    }
    else
    {
        *codeBuffer |= value << bitIndex;
    }
}

static byte GetBitInCode (const byte *codeBuffer, ulint bitIndex)
{
    codeBuffer += bitIndex >> 3;
    bitIndex = (byte) (7 - bitIndex & 7);
    return (byte) ((*codeBuffer & (1 << bitIndex)) != 0);
}

static ulint *CountBytesInInput (byte *input, ulint input_length)
{
    ulint *counters = calloc (256, sizeof (ulint));
    while (input_length--)
    {
        ++(counters [*input]);
        ++input;
    }

    return counters;
}

static lint NodePriorityComparator (const void *first, const void *second)
{
    return ((const NodeWithPriority *) first)->priority < ((const NodeWithPriority *) second)->priority ? 1 : -1;
}

static void InsertLeavesToHuffmanTree (PODVectorHandle treeNodes, byte *input, ulint input_length)
{
    NodeWithPriorityAndIndex nextNode = {0};
    PODVector_Insert (treeNodes, 0, (char *) &nextNode);

    ulint *counters = CountBytesInInput (input, input_length);
    short currentLeafValue = 0;

    while (currentLeafValue < 256)
    {
        if (counters [currentLeafValue] > 0)
        {
            nextNode.super.super.leafValue = (byte) currentLeafValue;
            nextNode.super.priority = counters [currentLeafValue];
            nextNode.index = (short) PODVector_Size (treeNodes);
            PODVector_Insert (treeNodes, PODVector_Size (treeNodes), (char *) &nextNode);
        }

        ++currentLeafValue;
    }

    free (counters);
}

static void SelectHuffmanTreeCapacity (PODVectorHandle treeNodes)
{
    ulint maximumExpectedSize = 0;
    ulint step = PODVector_Size (treeNodes);

    do
    {
        maximumExpectedSize += step;
        step = step / 2 + step % 2;
    }
    while (step > 1);

    maximumExpectedSize += 1;
    PODVector_Realloc (treeNodes, maximumExpectedSize);
}

/// Returns PODVector of nodes with priority and index. Last value in PODVector is root, 0 index is fake node.
static PODVectorHandle BuildHuffmanTreeForInput (byte *input, ulint input_length)
{
    PODVectorHandle treeNodes = PODVector_Create (INITIAL_TREE_NODES_CAPACITY, sizeof (NodeWithPriorityAndIndex));
    InsertLeavesToHuffmanTree (treeNodes, input, input_length);
    SelectHuffmanTreeCapacity (treeNodes);

    PVectorHandle heapVector = PVector_Create (INITIAL_TREE_NODES_CAPACITY);
    NodeWithPriority *currentNode = ((NodeWithPriority *) PODVector_Begin (treeNodes)) + 1;

    while (currentNode != (NodeWithPriority *) PODVector_End (treeNodes))
    {
        PVector_Insert (heapVector, PVector_End (heapVector), currentNode);
    }

    PVectorHandle heap = PVectorHeap_Heapify (heapVector, NodePriorityComparator);
    while (PVectorHeap_Size (heap) > 1)
    {
        NodeWithPriorityAndIndex *zeroChild = (NodeWithPriorityAndIndex *) PVectorHeap_Top (heap);
        PVectorHeap_Pop (heap);

        NodeWithPriorityAndIndex *oneChild = (NodeWithPriorityAndIndex *) PVectorHeap_Top (heap);
        PVectorHeap_Pop (heap);

        NodeWithPriorityAndIndex nextNode = {zeroChild->index, oneChild->index, 0,
                    zeroChild->super.priority + oneChild->super.priority, (short) PODVector_Size (treeNodes)};
        PODVector_Insert (treeNodes, PODVector_Size (treeNodes), (char *) &nextNode);
        PVectorHeap_Push (heap, PODVector_At (treeNodes, PODVector_Size (treeNodes) - 1));
    }

    PVectorHeap_Destruct (heap, ContainerCallback_NoAction);
    return treeNodes;
}

typedef struct
{
    PODVectorHandle treeNodes;
    ushort currentDepth;
    byte currentCode [MAX_HUFFMAN_CODE_LENGTH_IN_BYTES];
    output ByteCode *resultCodes;
} RecursiveGenerateByteCodeContext;

static void RecursiveGenerateByteCode (ushort nodeIndex, RecursiveGenerateByteCodeContext *context)
{
    ++context->currentDepth;
    Node *node = (Node *) PODVector_At (context->treeNodes, (ulint) nodeIndex);

    if (node->zeroChild || node->oneChild)
    {
        if (node->zeroChild != 0)
        {
            SetBitInCode (context->currentCode, context->currentDepth, 0);
            RecursiveGenerateByteCode (node->zeroChild, context);
        }

        if (node->oneChild != 0)
        {
            SetBitInCode (context->currentCode, context->currentDepth, 1);
            RecursiveGenerateByteCode (node->oneChild, context);
        }
    }
    else
    {
        context->resultCodes [node->leafValue].bitLength = context->currentDepth;
        memcpy (context->resultCodes [node->leafValue].code, context->currentCode, MAX_HUFFMAN_CODE_LENGTH_IN_BYTES);
    }

    --context->currentDepth;
}

static void GenerateByteCode (byte *input, ulint input_length, output ByteCode *resultCodes)
{
    RecursiveGenerateByteCodeContext context;
    context.treeNodes = BuildHuffmanTreeForInput (input, input_length);
    context.currentDepth = 0;
    context.resultCodes = resultCodes;

    RecursiveGenerateByteCode ((ushort) (PODVector_Size (context.treeNodes) - 1), &context);
    PODVector_Destruct (context.treeNodes, ContainerCallback_NoAction);
}

static void WriteEncodedByte (byte *encoded, ulint *bitIndex, byte toCode, ByteCode *codes)
{
    ushort codeBitIndex = 0;
    while (codeBitIndex < codes [toCode].bitLength)
    {
        SetBitInCode (encoded, *bitIndex, GetBitInCode (codes [toCode].code, codeBitIndex));
        ++codeBitIndex;
        ++*bitIndex;
    }
}

byte *Huffman_Encode (byte *input, ulint input_length, output ulint *codeSize, output ByteCode *resultCodes)
{
    ulint bitIndex = 0;
    ulint capacity = input_length;
    byte *encoded = malloc (input_length);
    GenerateByteCode (input, input_length, resultCodes);

    while (input_length--)
    {
        if (((bitIndex + resultCodes [*input].bitLength) >> 3) >= capacity - 1)
        {
            capacity *= 2;
            encoded = realloc (encoded, capacity);
        }

        WriteEncodedByte (encoded, &bitIndex, *input, resultCodes);
        ++input;
    }

    *codeSize = (bitIndex >> 3) + ((bitIndex & 7) > 0);
    encoded = realloc (encoded, *codeSize);
    return encoded;
}

/// Returns PODVector of nodes. First node ([0]) index is fake node, second ([1]) is root.
PODVectorHandle ConstructTrie (ByteCode *codes)
{
    PODVectorHandle trie = PODVector_Create (INITIAL_TREE_NODES_CAPACITY, sizeof (Node));
    Node toInsert = {0, 0, 0};
    PODVector_Insert (trie, 0, (char *) &toInsert);

    ushort byteValue = 0;
    while (byteValue < 256)
    {
        if (codes [byteValue].bitLength > 0)
        {
            Node *currentNode = (Node *) PODVector_At (trie, 1);
            ushort bitIndex = 0;

            while (bitIndex < codes [byteValue].bitLength)
            {
                byte bit = GetBitInCode (codes [byteValue].code, bitIndex);
                ++bitIndex;

                if (*(&(currentNode->zeroChild) + bit) == 0)
                {
                    *(&(currentNode->zeroChild) + bit) = (ushort) PODVector_Size (trie);
                    PODVector_Insert (trie, PODVector_Size (trie), (char *) &toInsert);
                }

                currentNode = (Node *) PODVector_At (trie, *(&(currentNode->zeroChild) + bit));
            }

            currentNode->leafValue = (byte) byteValue;
        }

        ++byteValue;
        ++codes;
    }

    return trie;
}

byte *Huffman_Decode (byte *input, ulint input_length, ulint *maxDecodedSize, ByteCode *codes)
{
    PODVectorHandle trie = ConstructTrie (codes);
    byte *decoded = malloc (*maxDecodedSize);
    ulint decodedSize = 0;
    ulint bitIndex = 0;

    while (decodedSize < *maxDecodedSize)
    {
        Node *currentNode = (Node *) PODVector_At (trie, 1);
        while (currentNode->zeroChild != 0 || currentNode->oneChild != 0)
        {
            if ((bitIndex >> 3) >= input_length)
            {
                break;
            }

            byte bit = GetBitInCode (input, bitIndex);
            ++bitIndex;

            // TODO: Error handling?
            currentNode = (Node *) PODVector_At (trie, *(&(currentNode->zeroChild) + bit));
        }

        if (currentNode->zeroChild == 0 && currentNode->oneChild == 0)
        {
            decoded [decodedSize] = currentNode->leafValue;
            ++decodedSize;
        }
        else
        {
            break;
        }
    }

    *maxDecodedSize = decodedSize;
    PODVector_Destruct (trie, ContainerCallback_NoAction);
    return decoded;
}
