#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct Heap* createMinHeap(unsigned capacity) {
    struct Heap* heap = (struct Heap*)malloc(sizeof(struct Heap));
    heap->length = 0;
    heap->capacity = capacity;
    heap->array = (struct BSTNode**)malloc(heap->capacity * sizeof(struct BSTNode*));
    return heap;
}

int checkingSize(struct Heap* heap) {

    return (heap->length == 1);
}

void swapNodes(struct BSTNode** var1, struct BSTNode** var2){

    struct BSTNode* var3 = *var1;
    *var1 = *var2;
    *var2 = var3;
}

struct BSTNode* newNode(char *word, unsigned frequency) {
    struct BSTNode* temp = (struct BSTNode*)malloc(sizeof(struct BSTNode));

    temp->left = temp->right = NULL;
    temp->word = word;
    temp->frequency = frequency;

   return temp;
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

void printArray(int array[], int length,int fd) {
    int i;

    char code[25];
    strcpy(code,"");
    for (i = 0; i < length; ++i){
        if(array[i]==1)
                strcat(code,"1");
        else
                strcat(code,"0");
   //     printf("%d", array[i]); 
    }
        write(fd,code,strlen(code));

 //     printf("\n"); 
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

void printHuffman(struct BSTNode* root, int array[], int up,int fd) {

    if (root->left) {

        array[up] = 0;
        printHuffman(root->left, array, up + 1,fd);
    }

    if (ChildNode(root)) {
        printArray(array,up,fd);        
        write(fd,"\t",1);
        write(fd,root->word,strlen(root->word));
        write(fd,"\n",1);
    /*    printf("%s: ", root->word);
        printArray(array,up,fd);
*/
    }

    if (root->right) {

        array[up] = 1;
        printHuffman(root->right, array, up + 1,fd);
    }
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

void Codes(char *word[], int frequency[], int length) {

    struct BSTNode* root = HuffmanTree(word, frequency, length);

    int array[height], up = 0;
    int fd=open("HuffmanCodebook", O_CREAT|O_WRONLY,0600);
    write(fd,"\\\n",2);
    printHuffman(root, array, up,fd);
    close(fd);
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
