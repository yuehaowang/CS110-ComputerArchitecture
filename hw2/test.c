#include "doubll.h"
#include <stdio.h>


void display_list (doubll* list)
{
    doubll_item* temp = &(list -> head);

    while (temp != NULL) {
        doubll_item* next = temp -> next;

        if (temp -> prev == NULL) {
            printf("head <--> ");
        } else if (temp -> next == NULL) {
            printf("tail\n");
        } else {
            printf("%d <--> ", *(int*)(temp -> data));
        }

        temp = next;
    }
}

bool less (const void* d1, const void* d2)
{
    return *(int*)d1 < *(int*)d2;
}

int main()
{
    doubll ll;
    
    int data_set[] = {1, 3, 45234, 62, 9, 89, 91, 40, 100, 178};
    doubll_item* item_set[10];
    int i;

    list_init(&ll);
    
    for (i = 0; i < 10; i++) {
        item_set[i] = insert_list(&ll, &(ll.head), &data_set[i], 4);
    }

    display_list(&ll);
    
    remove_item(&ll, item_set[3]);
    display_list(&ll);
    sort_list(&ll, &less);

    remove_item(&ll, item_set[1]);
    display_list(&ll);

    remove_item(&ll, item_set[0]);
    display_list(&ll);

    item_set[3] = insert_list(&ll, item_set[5], &data_set[3], 4);
    sort_list(&ll, &less);
    display_list(&ll);

    purge_list(&ll);
    display_list(&ll);

    return 0;
}