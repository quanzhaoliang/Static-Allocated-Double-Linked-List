#include <stdio.h>
#include <assert.h>
#include "list.h"

static Node nodePool[LIST_MAX_NUM_NODES];
static List headPool[LIST_MAX_NUM_HEADS];

static Node* freeNodePool = &nodePool[0];
static List* freeHeadPool = &headPool[0];

static bool poolsInitialized = false;

// Function to initialize the node and list head pools
static void initialize() {
    if (!poolsInitialized) {
        for (int i = 0; i < LIST_MAX_NUM_NODES - 1; i++) {
            nodePool[i].nextFree = &nodePool[i + 1];
        }
        nodePool[LIST_MAX_NUM_NODES - 1].next = NULL;

        for (int i = 0; i < LIST_MAX_NUM_HEADS - 1; i++) {
            headPool[i].next = &headPool[i + 1];
        }
        headPool[LIST_MAX_NUM_HEADS - 1].next = NULL;

        poolsInitialized = true;
    }
}

//free the removed node
void freeNode(Node* node){
    node->next = NULL;
    node->prev = NULL;
    node->nextFree = freeNodePool;
}

void freeNodeItem(void* item){
    item = NULL;
}


// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
void List_free(List* pList, FREE_FN pItemFreeFn){
    assert(pList != NULL);

    Node* currentNode = pList->head;
    while (currentNode != NULL)
    {
        Node* nextNode = currentNode->next;
        (*pItemFreeFn)(currentNode->item);
        freeNode(currentNode);
        currentNode = nextNode;
    }
    pList->head = NULL;
    pList->tail = NULL;
    pList->current = NULL;
    pList->size = 0;
    pList->outOfBounds = -1;
    pList->next = freeHeadPool;
}

List* List_create() {
    initialize();
    
    // Get a free list head from the pool
    List* new_list = freeHeadPool;
    freeHeadPool = freeHeadPool->next;

    // Initialize the new list
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->current = NULL;
    new_list->outOfBounds = LIST_OOB_START;
    new_list->size = 0;

    return new_list;
}

int List_count(List* pList){
    return pList->size;
}

void* List_first(List* pList){
    if (pList->head != NULL){
        pList->current = pList->head;
        return pList->head->item;
    }
    pList->current = NULL;
    return NULL;
}

void* List_last(List* pList) {
    if (pList->tail != NULL) {
        pList->current = pList->tail;
        return pList->tail->item;
    }
    pList->current = NULL;
    return NULL;
}

void* List_next(List* pList) {
    if (pList->current != NULL) {
        if (pList->current->next != NULL) {
            pList->current = pList->current->next;
            return pList->current->item;
        } else {
            pList->current = NULL;
            pList->outOfBounds = LIST_OOB_END;
        }
    }
    if (pList->outOfBounds == LIST_OOB_START){
        pList->current = pList->head;
        return pList->current->item;
    }
    return NULL;
}

void* List_prev(List* pList) {
    if (pList->current != NULL) {
        if (pList->current->prev != NULL) {
            pList->current = pList->current->prev;
            return pList->current->item;
        } else {
            pList->current = NULL;
            pList->outOfBounds = LIST_OOB_START;
        }
    }
    if (pList->outOfBounds == LIST_OOB_END){
        pList->current = pList->tail;
        return pList->current->item;
    }
    
    return NULL;
}

void* List_curr(List* pList) {
    if (pList->current != NULL) {
        return pList->current->item;
    }
    return NULL;
}

int List_insert_after(List* pList, void* pItem){
    Node* newNode = freeNodePool;
    if (newNode == NULL){
        return -1;
    }

    freeNodePool = freeNodePool->nextFree;

    newNode->item = pItem;

    // insert ot empty list
    if (pList->size == 0){
        pList->head = newNode;
        pList->tail = newNode;
    }

    //if current pointer is out of bound
    if (pList->current == NULL) {
        if (pList->outOfBounds == LIST_OOB_START) {
            // insert to head
            pList->head->prev = newNode;
            newNode->next = pList->head;
            pList->head = newNode;
        } else {
            // insert to tail
            pList->tail->next = newNode;
            newNode->prev = pList->tail;
            pList->tail = newNode;
        }
    } 
    
    else {
        // insert after current
        newNode->next = pList->current->next;
        newNode->prev = pList->current;
        pList->current->next = newNode;
        if (newNode->next != NULL) {
            newNode->next->prev = newNode;
        }
    }

    pList->current = newNode;
    pList->size++;
    return 0;
}

int List_insert_before(List* pList, void* pItem) {
    Node* newNode = freeNodePool;
    if (newNode == NULL) {
        return -1;
    }

    freeNodePool = freeNodePool->nextFree;
    newNode->item = pItem;

       // insert ot empty list
    if (pList->size == 0){
        pList->head = newNode;
        pList->tail = newNode;
    }

    //if current pointer is out of bound
    if (pList->current == NULL) {
        if (pList->outOfBounds == LIST_OOB_START) {
            // insert to head
            pList->head->prev = newNode;
            newNode->next = pList->head;
            pList->head = newNode;
        } else {
            // insert to tail
            pList->tail->next = newNode;
            newNode->prev = pList->tail;
            pList->tail = newNode;
        }
    } 
    
    else {
        // insert before current
        newNode->prev = pList->current->prev;
        newNode->next = pList->current;
        pList->current->prev = newNode;
        if (newNode->prev != NULL) {
            newNode->prev->next = newNode;
        }
    }

    pList->current = newNode;
    pList->size++;
    return 0;
}

int List_append(List* pList, void* pItem) {
    Node* newNode = freeNodePool;
    if (newNode == NULL) {
        return -1;
    }

    freeNodePool = freeNodePool->nextFree;
    newNode->item = pItem;

    if (pList->size != 0) {
        //add item to the end of the list
        pList->tail->next = newNode;
        newNode->prev = pList->tail;
        pList->tail = newNode;
    } else {
        // add to Empty list
        pList->head = newNode;
        pList->tail = newNode;
    }

    pList->current = newNode;
    pList->size++;
    return 0;
}

int List_prepend(List* pList, void* pItem) {
    Node* newNode = freeNodePool;
    if (newNode == NULL) {
        return -1;
    }

    freeNodePool = freeNodePool->nextFree;
    newNode->item = pItem;

    if (pList->size != 0) {
        //add item to the start of the list
        pList->head->prev = newNode;
        newNode->next = pList->head;
        pList->head = newNode;
    } else {
        // add to empty list
        pList->head = newNode;
        pList->tail = newNode;
    }

    pList->current = newNode;
    pList->size++;
    return 0;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
    assert(pList != NULL);

    // If current is in bound
    if (pList -> current != NULL){
        Node* removedNode = pList->current;
        void* removedItem = removedNode->item;

        if (pList->size == 1){
            pList->head = NULL;
            pList->tail = NULL;
            pList->current = NULL;
            pList->outOfBounds = LIST_OOB_END;
        }
        else{
            //if the removed node is not the head or tail of the list
            if (removedNode->prev != NULL || removedNode->next != NULL){
                pList->current = pList->current->next;
                removedNode->prev->next = removedNode->next;
                removedNode->next->prev = removedNode->prev;
            }
        
            else{
                //if the removed node is the head of the list
                if (removedNode->prev == NULL){
                    pList->head = removedNode->next;
                    pList->head->prev = NULL;
                    pList->current = pList->head;
                }
                //if the removed node is the tail of the list
                else{
                    pList->tail = removedNode->prev;
                    removedNode->next = NULL;
                    pList->current = NULL;
                    pList->outOfBounds = LIST_OOB_END;
                }
            }
        }

        pList->size--;
        freeNode(removedNode);
        return removedItem;
    }
    return NULL;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
    assert(pList != NULL);

    if (pList -> tail != NULL){
        Node* removedNode = pList->tail;
        void* removedItem = removedNode->item;

        if (pList->size == 1){
            pList->head = NULL;
            pList->tail = NULL;
            pList->current = NULL;
            pList->outOfBounds = LIST_OOB_START;
        }

        else{
            pList->tail->prev->next = NULL;
            pList->tail = pList->tail->prev;
            pList->current = pList->tail;
        }

        freeNode(removedNode);
        pList->size--;
        return removedItem;
    }
    return NULL;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){
    assert(pList1 != NULL);
    assert(pList2 != NULL);

    pList1->tail->next = pList2->head;
    pList2->head->prev = pList1->tail;
    pList1->tail = pList2->tail;
    pList1->size += pList2->size;

    List_free(pList2, freeNodeItem);
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
bool comparator(void* pItem, void* pComparisonArg){
    int* item = (int*)pItem;
    int* target = (int*)pComparisonArg;
    return (*item == *target);
}
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    assert(pList != NULL);

    Node* currentNode = pList->current;
    if (pList->outOfBounds == LIST_OOB_START){
        currentNode = pList->head;
    }

    while (currentNode != NULL)
    {
        if (pComparator(currentNode->item, pComparisonArg)){
            pList->current = currentNode;
            return currentNode->item;
        }
        currentNode = currentNode->next;
    }
    pList->current = NULL;
    pList->outOfBounds = LIST_OOB_END;
    return NULL;
}
