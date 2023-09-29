#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include <assert.h>

void freeNodeItem(void* item){
    item = NULL;
}


void printList(List* pList){
    Node* current;
    int count = 0;
    for (current = pList->head; current != NULL ; current = current->next ){
        count++;
        printf("%d-", *(int*)(current->item));
    }
    printf("\n");
    printf("the size of list is: %d, same as list->size: %d \n",count, pList->size);
    printf("current pointer item is: %d", *(int*)(pList->current->item));
    printf("\n");
}

bool test1(){
    bool flag = true;
    List* list = List_create();

    //Test append and prepend
    int item1 = 1;
    int item2 = 2;
    int item3 = 3;

     //2-1-3
    List_append(list, &item1);
    List_prepend(list, &item2);
    List_append(list, &item3);
   

    int item4 = 5;
    int item5 = 7;
    int item6 = 11;
    
    //11-7-5-2-1-3
    List_prepend(list, &item4);
    List_prepend(list, &item5);
    List_prepend(list, &item6);
   
    //11-7-5-2-1-3-13-17
    int item7 = 13;
    int item8 = 17;
    List_append(list, &item7);
    List_append(list, &item8);

    int arr[8] = {11,7,5,2,1,3,13,17};
    Node* current;
    int i = 0;
    for (current = list->head; current != NULL ; current = current->next ){
        if (*(int*)(current->item) != arr[i] || *(int*)(list->current->item) != 17){
            flag = false;
        }
        i++;
    }
    if (flag == true){
        printf("append and prepend ok\n");
    }
    
    //test List_first
    void* pointerFirst = List_first(list);
    if (*(int*)(pointerFirst) != 11 || *(int*)(list->current->item) != 11){
        flag = false;
    }
    if (flag == true){
        printf("List_first ok\n");
    }

    //test Last_first
    void* pointerLast = List_last(list);
    if (*(int*)(pointerLast) != 17 || *(int*)(list->current->item) != 17){
        flag = false;
    }
    if (flag == true){
        printf("List_last ok\n");
    }

    //11-7-5-2-1-3-13-17
    //Test List_next
    void* item = List_next(list);
    if (item != NULL || list->outOfBounds != 1){
        flag = false;
    }
    item = List_next(list);
    if (item != NULL || list->outOfBounds != 1){
        flag = false;
    }

    item = List_prev(list);
    if (*(int*)item != 17){
        flag = false;
    }

    for (int i = 0; i < 7; i++){
        item = List_prev(list);
    }
    if (*(int*)item != 11 || *(int*)(list->current->item) != 11){
        flag = false;
    }

    item = List_prev(list);
    if (item != NULL || list->outOfBounds != 0){
        flag = false;
    }

    item = List_next(list);
    if (*(int*)item != 11 || *(int*)(list->current->item) != 11){
        flag = false;
    }

    if (flag == true){
        printf("List_next ok\n");
        printf("List_prev ok\n");
    }

    if (flag == false){
        printf("Error occurred!\n");
    }
    else{
        printf("test1 passed\n");
    }

    List_free(list, freeNodeItem);
    return flag;
}

bool test2(){
    List* list1 = List_create();
    int item1 = 1;
    int item2 = 2;
    int item3 = 3;
    List_append(list1, &item1);
    List_append(list1, &item2);
    List_append(list1, &item3);

    //1-2-3
    List_prev(list1);
    
    //1-2-4-3
    int item4 = 4;
    List_insert_after(list1, &item4);
    
    int item5 = 5;
    int item6 = 6;
    List_next(list1);
    //1-2-4-3-5
    List_insert_after(list1, &item5);
    printf("%d\n", *(int*)List_curr(list1));
    List_insert_before(list1, &item6);
    

    int item7 = 7;
    int item8 = 8;
    int item9 = 9;

    List* list2 = List_create();
    List_append(list2, &item7);
    List_next(list2);
    List_insert_before(list2, &item8);
    List_prev(list2);
    List_prev(list2);
    List_insert_before(list2, &item9);
    List_concat(list1, list2);
    printList(list1);
    
    List_free(list1, freeNodeItem);
    return true;

}

bool test3(){
    bool flag = true;
    List* list = List_create();
	List_insert_after(list, "Hello");
	List_insert_after(list, "World");
	List_insert_after(list, "!");
	List_first(list);
	List_insert_before(list, "Hello1");
	List_insert_before(list, "World1");
	List_insert_before(list, "!");
	
	List_first(list);
    char* arr[] = { "World1", "Hello1", "Hello", "World", "!"};
	for (int i = 0; i < 5; i++) {
		if ((char*)List_next(list) != arr[i]){
            flag = false;
        }
	}

    if (flag == true){
        printf("insert_after & insert_before OK\n");
    }
    else{
        printf("insert_after & insert_before Error\n");
        return flag;
    }
    
	for (int i = 0; i < 9; i++) {
		List_create();
	}
	if (List_create() != NULL){
        flag = false;
    }
	for (int i = 0; i < LIST_MAX_NUM_NODES - 5; i++){
		List_insert_after(list, "a");
    }
    if (List_insert_after(list, "v") != -1){
        flag = false;
    }
	
    if (flag == true){
        printf("headPool and nodePool OK\n");
        printf("test3 passed\n");
    }
    else{
        printf("headPool and nodePool Error\n");
        return flag;
    }

    List_free(list, freeNodeItem);
    return flag;
}



int main() {

    test1();
    test2();
    test3();
    return 0;
}