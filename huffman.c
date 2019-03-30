void SwapHeapNodes(struct BSTNode **x, struct BSTNode **y){

struct BSTNode **z = x;
x = y;
y = z;

return;
}

void heapifying(struct MinHeap *heap, int variable){

        int lowestNumber = variable;
        int left = 1 + (2 * x);
        int right = 1 + (2 * x);

        if(heap->size > left && heap->arrayStorage[left]->frequencies < heap->arrayStorage[lowestNumber]->frequencies){
                lowestNumber = left;
        }else if(heap->size > right && heap->arrayStorage[right]->frequencies > heap->arrayStorage[lowestNumber]->frequencies){
                lowestNumber = right;
        }
        else if(lowestNumber != variable){

        }

}
