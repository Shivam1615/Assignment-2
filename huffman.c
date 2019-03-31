#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

struct BSTNode* newNode(char *word, unsigned frequency) {
    struct BSTNode* temp = (struct BSTNode*)malloc(sizeof(struct BSTNode));

    temp->left = temp->right = NULL;
    temp->word = word;
    temp->frequency = frequency;

   return temp;
}

struct Heap* createMinHeap(unsigned capacity) {
    struct Heap* heap = (struct Heap*)malloc(sizeof(struct Heap));
    heap->length = 0;
    heap->capacity = capacity;
    heap->array = (struct BSTNode**)malloc(heap->capacity * sizeof(struct BSTNode*));
    return heap;
}
void swapNodes(struct BSTNode** var1, struct BSTNode** var2){

    struct BSTNode* var3 = *var1;
    *var1 = *var2;
    *var2 = var3;
}

void heapify(struct Heap* heap, int variable) {

    int lowestNumber = variable;
    int left = 1 + (2 * variable);
    int right = 2 + (2 * variable);

    if (left < heap->length && heap->array[left]->frequency < heap->array[lowestNumber]->frequency)
        lowestNumber = left;

    if (right < heap->length && heap->array[right]->frequency < heap->array[lowestNumber]->frequency)
        lowestNumber = right;

    if (lowestNumber != variable) {
        swapNodes(&heap->array[lowestNumber], &heap->array[variable]);
        heapify(heap, lowestNumber);
    }
}

int checkingSize(struct Heap* heap) {

    return (heap->length == 1);
}

struct BSTNode* min(struct Heap* heap) {

    struct BSTNode* temp = heap->array[0];
    heap->array[0] = heap->array[heap->length - 1];
    --heap->length;
    heapify(heap, 0);
    return temp;
}
void insertMinHeap(struct Heap* heap, struct BSTNode* BSTNode) {
    ++heap->length;
    int i = heap->length - 1;

    while (i && BSTNode->frequency < heap->array[(i - 1) / 2]->frequency) {

        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    heap->array[i] = BSTNode;
}

void buildMinHeap(struct Heap* heap) {

    int var = heap->length - 1;
    int i;

    for (i = (var - 1) / 2; i >= 0; --i)
        heapify(heap, i);
}

void printArr(int array[], int length) {
    int i;
    for (i = 0; i < length; ++i)
        printf("%d", array[i]);
        printf("\n");
}

int ChildNode(struct BSTNode* root) {

    return !(root->left) && !(root->right);
}

struct Heap* createAndBuildMinHeap(char *word[], int frequency[], int length) {

    struct Heap* Heap = createMinHeap(length);
        int i;
    for (i = 0; i < length; ++i)
        Heap->array[i] = newNode(word[i], frequency[i]);
        Heap->length = length;
        buildMinHeap(Heap);

        return Heap;
}

struct BSTNode* HuffmanTree(char *word[], int frequency[], int length) {
    struct BSTNode *left, *right, *up;

    struct Heap* heap = createAndBuildMinHeap(word, frequency, length);
while (!checkingSize(heap)) {

        left = min(heap);
        right = min(heap);
        char value[2];
        strcpy(value,"$");
        up = newNode(value, left->frequency + right->frequency);

        up->left = left;
        up->right = right;

        insertMinHeap(heap, up);
    }

    return min(heap);
}

void printHuffman(struct BSTNode* root, int array[], int up) {

    if (root->left) {

        array[up] = 0;
        printHuffman(root->left, array, up + 1);
    }

    if (root->right) {

        array[up] = 1;
        printHuffman(root->right, array, up + 1);
    }

    if (ChildNode(root)) {

        printf("%s: ", root->word);
        printArr(array, up);
    }
}

void Codes(char *word[], int frequency[], int length) {

    struct BSTNode* root = HuffmanTree(word, frequency, length);

    int array[MAX_TREE_HT], up = 0;

    printHuffman(root, array, up);
}

void getCodeBook(char *word[], int freqs[],int wordAmount) {

        char *array[wordAmount];
        int i;
        for(i=0;i<wordAmount;i++){
                array[i]=word[i];

        }
        int length = sizeof(array) / sizeof(array[0]);

        Codes(array, freqs, length);

    return;
}
