#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100 

struct BSTNode {
    char *word;
    unsigned frequency;
    struct BSTNode *left, *right;
};

struct Heap {
    unsigned length;
    unsigned capacity;
    struct BSTNode** array;
};

struct BSTNode* newNode(char *word, unsigned frequency);

struct Heap* createMinHeap(unsigned capacity);

void swapNodes(struct BSTNode** a, struct BSTNode** b);

void heapify(struct Heap* heap, int variable);

int checkingSize(struct Heap* heap);

struct BSTNode* min(struct Heap* heap);

void insertMinHeap(struct Heap* heap,struct BSTNode* BSTnode);

void buildMinHeap(struct Heap* heap);

void printArr(int array[], int length);

int ChildNode(struct BSTNode* root);

struct Heap* createAndBuildMinHeap(char *word[], int frequency[], int length);

struct BSTNode* HuffmanTree(char *word[], int frequency[], int length);

void printHuffman(struct BSTNode* root, int array[], int up);

void Codes(char *word[], int frequency[], int length);

void getCodeBook();
