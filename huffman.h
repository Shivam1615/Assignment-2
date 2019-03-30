#include<stdio.h>
#include<stdlib.h>

struct BSTNode{

int frequencies;
char *word;
struct Node *left;
struct Node * right;
};

struct MinHeap{

int capacity;
int size;
struct minHeap **arrayStorage;
};

struct BSTNode* creatingNodes(char data, int frequencies){

struct BSTNode* temp = (struct MinHeapNode*)(sizeof(struct MinHeapNode*));
temp->left = NULL;
temp->right = NULL;
temp->frequencies = frequencies;
temp->word = data;

return temp;
}

struct MinHeap *CreatingMinHeap(int capacities){

struct MinHeap *heap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
heap->size = 0;
heap->capacity = capacities;
heap->arrayStorage = (struct BSTNode**)malloc(heap->capacity * sizeof(struct BSTNode*));

return heap;
}
