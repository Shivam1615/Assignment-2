#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define height 100 

typedef struct data{
	char *word;
	int freq;
} data;

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

void insertMinHeap(struct Heap* heap,struct BSTNode* BSTnode);

void buildMinHeap(struct Heap* heap);

void printArray(int array[], int length,int fd);

void swapNodes(struct BSTNode** a, struct BSTNode** b);

void heapify(struct Heap* heap, int variable);

int checkingSize(struct Heap* heap);

struct BSTNode* min(struct Heap* heap);

int ChildNode(struct BSTNode* root);

struct Heap* createAndBuildMinHeap(char *word[], int frequency[], int length);

struct BSTNode* newNode(char *word, unsigned frequency);

struct Heap* createMinHeap(unsigned capacity);

struct BSTNode* HuffmanTree(char *word[], int frequency[], int length);

void printHuffman(struct BSTNode* root, int array[], int up,int fd);

void Codes(char *word[], int frequency[], int length);

void getCodeBook(char *word[],int freqs[],int wordAmount);
