# CUtils
Simple containers and sorts implementation in pure C.
Modules list:
1. CContainers (with tests and speed-check submodules).
2. CAlgorithm (with tests and speed-check sudmodules).
3. HuffmanCoding (with tests and executables submodules).

## CContainers
### CContainers (library)
#### Implemented containers
* PDoubleLinkedList -- double linked list for pointers.
* PHashMap -- hash map implementation for <pointer, pointer> pairs.
* PHeap -- binary heap for pointers with customizable base container.
* PODVector -- dynamic array for POD types.
* PSinleLinkedList -- single linked list for pointers.
* PVector -- dynamic array for pointers.
* PVectorHeap -- binary heap for pointers, based on PVector.

#### Interfaces
Interface-like structures, used to simulate polymorphism.

#### Utils
Error handling, for-each and for-each-reversed implementations.

### CContainersTests
CUnit framework tests for CContainers.

### CContainersSpeedCheck
Speed-check executable for CContainers methods.

## CAlgortihm
### CAlgorithm (library)
#### Sort
Sorts implementations for containers that support interfaces provided by CContainers:
* HeapSort.
* MergeSort.
* InplaceMergeSort.
* QuickSort.
* IntroSort.

#### PArraySort
Sorts implementations for plain pointer arrays:
* HeapSort.
* MergeSort.
* InplaceMergeSort.
* QuickSort.
* IntroSort.

### CAlgorithmTests
CUnit framework tests for CAlgorithm sorts.

### CAlgortihmSpeedCheck
Speed-check executable for CAlgorithm sorts.

## HuffmanCoding
### HuffmanCoding (library)
Simple one-header-one-object huffman encoding/decoding implementation using CContainers.
### HuffmanCodingTests
Basic coding/decoding tests for the library. CUnit framework used.
### HuffmanArchive
Console util for coding/decoding files using huffman algorithm.
### HuffmanArchiveParallel
Parallel version of HuffmanArchive: additional worker threads used for parallel coding/decoding file parts. This util is about 2 times faster than non-parallel version.
