#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include <assert.h>

void freeNodeItem(void* item){
    item = NULL;
}

bool comparatorChar(void* pItem, void* pComparisonArg){
    char* item1 = (char*)pItem;
    char* item2 = (char*)pComparisonArg;
    return (*item1 == *item2);
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
    bool flag = true;
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

    int arr[9] = {1,2,4,3,6,5,9,7,8};
    Node* current;
    int i = 0;
    for (current = list1->head; current != NULL ; current = current->next ){
        if (*(int*)(current->item) != arr[i]){
            flag = false;
        }
        i++;
    }
    if (flag == true){
        printf("List_concat ok\n");
        printf("test2 passed\n");
    }
    else{
        printf("List_concat Error\n");
        printf("test2 Error\n");
    }
    
    List_free(list1, freeNodeItem);
    return true;

}

bool test3(){
    bool flag = true;
    List* list = List_create();
	List_insert_after(list, "Who");
	List_insert_after(list, "are");
	List_insert_after(list, "you");
	List_first(list);
	List_insert_before(list, "!");
	List_insert_before(list, "Liang");
	List_insert_before(list, "Sheldon");
	
	List_first(list);
    char* arr[] = { "Liang", "!", "Who", "are", "you"};
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

bool test4(){
    bool flag = true;
    List* list = List_create();
    List_append(list, "W");
    List_append(list, "O");
    List_append(list, "R");
    List_append(list, "L");
    List_append(list, "D");
    List_last(list);
    List_next(list);
    List_prepend(list, "H");
    List_insert_after(list, "E");
    List_insert_after(list, "L");
    List_insert_after(list, "L");
    List_insert_after(list, "O");
   // H-E-L-L-O-W-O-R-L-D

    printf("\n");
    List_first(list);
    List_prev(list);
    List_remove(list);

    List_next(list);
    List_next(list);
    List_next(list);
    List_remove(list);
    List_remove(list);
    List_remove(list);
    List_remove(list);
    List_remove(list);
    List_trim(list);
    List_trim(list);
    //H-E-R
    
    char* arr[] = {"H", "E", "R"};
    Node* current;
    int i = 0;
    for (current = list->head; current != NULL ; current = current->next ){
        if ((char*)(current->item) != arr[i]){
            flag = false;
        }
        i++;
    }
   
    if (flag == true){
        printf("trim and remove OK\n");
        printf("test4 passed\n");
    }
    else{
        printf("trim and remove Error\n");
        printf("test4 Error\n");   
    }

    List_free(list, freeNodeItem);
    return flag;
}

bool test5(){   

    bool flag = true;
    List* list = List_create();

    List_append(list, "W");
    List_append(list, "O");
    List_append(list, "R");
    List_append(list, "L");
    List_append(list, "D");
    List_last(list);
    List_next(list);
    List_prepend(list, "H");
    List_insert_after(list, "E");
    List_insert_after(list, "L");
    List_insert_after(list, "L");
    List_insert_after(list, "O");
   // H-E-L-L-O-W-O-R-L-D

    List_first(list);
    List_prev(list);
    bool flag1 = List_search(list, comparatorChar, "R");
    bool flag2 = List_search(list, comparatorChar, "H");
    if (flag1 == true && flag2 == false && list->outOfBounds == 1){
        printf("List_search OK\n");
        printf("test5 passed\n");
    }
    else{
        printf("List_search Error\n");
        printf("test5 Error\n");
    }

    List_free(list, freeNodeItem);
    return flag;
}



int main() {

    test1();
    printf("\n");
    test2();
    printf("\n");
    test3();
    printf("\n");
    test4();
    printf("\n");
    test5();
    return 0;
}