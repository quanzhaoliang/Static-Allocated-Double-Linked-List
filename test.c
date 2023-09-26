#include <stdio.h>
#include <stdbool.h>
#include "list.h"

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

    if (flag == true){
        printf("List_next ok\n");
    }


    if (flag == false){
        printf("Error occurred!");
    }
    else{
        printf("test1 passed");
    }
    return flag;
}

int main() {
    test1();
    return 0;
}