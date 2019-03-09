#include "doubll.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*********************** Helpers ***********************/

static void move_item (doubll_item* prev, doubll_item* temp)
{
    if (temp -> prev != NULL) temp -> prev -> next = temp -> next;
    if (temp -> next != NULL) temp -> next -> prev = temp -> prev;

    temp -> prev = prev;
    temp -> next = prev -> next;
    if (prev -> next != NULL) prev -> next -> prev = temp;
    prev -> next = temp;
}

#ifdef CHECK_LIST
static bool is_item_in_list (doubll* list, doubll_item* item)
{
    doubll_item* temp;
    
    temp = &(list -> head);
    while (temp != NULL) {
        if (temp == item) return true;
        temp = temp -> next;
    }

    return false;
}
#endif

static bool invalid_list (doubll* list)
{
    return (list == NULL) || (list -> head).prev != NULL || (list -> head).next == NULL || (list -> tail).next != NULL || (list -> tail).prev == NULL;
}


/*********************** Implementation ***********************/

void list_init (doubll *list)
{
    (list -> head).next = &(list -> tail);
    (list -> head).prev = NULL;
    (list -> head).data = NULL;
    (list -> head).size = 0;
    (list -> tail).prev = &(list -> head);
    (list -> tail).next = NULL;
    (list -> tail).data = NULL;
    (list -> tail).size = 0;
    
    list -> items = 0;
}

doubll_item* list_begin (doubll* list)
{
    if (invalid_list(list)) return NULL;
    return (list -> head).next;
}

doubll_item* list_head (doubll* list)
{
    if (invalid_list(list)) return NULL;
    return &(list -> head);
}

doubll_item* list_end (doubll* list)
{
    if (invalid_list(list)) return NULL;
    return &(list -> tail);
}

size_t list_size (doubll* list)
{
    if (invalid_list(list)) return -1;
    return list -> items;
}

doubll_item* insert_list (doubll* list, doubll_item* item, void* data, size_t size)
{
    doubll_item* new_item;

    if (invalid_list(list) || item == NULL || item -> next == NULL || data == NULL || size == 0) return NULL;

    #ifdef CHECK_LIST
    if (!is_item_in_list(list, item)) return NULL;
    #endif

    new_item = (doubll_item*)malloc(sizeof(doubll_item));
    if (new_item == NULL) return NULL;
    new_item -> data = (void*)malloc(size);
    if (new_item -> data == NULL) return NULL;
    memcpy(new_item -> data, data, size);
    new_item -> size = size;

    new_item -> prev = item;
    new_item -> next = item -> next;
    item -> next -> prev = new_item;
    item -> next = new_item;

    (list -> items)++;

    return new_item;
}

doubll_item* remove_item (doubll* list, doubll_item* item)
{
    doubll_item* next;

    if (invalid_list(list) || item == NULL || item -> prev == NULL || item -> next == NULL) return NULL;

    #ifdef CHECK_LIST
    if (!is_item_in_list(list, item)) return NULL;
    #endif

    next = item -> next;

    item -> prev -> next = item -> next;
    item -> next -> prev = item -> prev;

    if(item -> data != NULL) free(item -> data);
    item -> data = NULL;
    free(item);

    (list -> items)--;

    return next;
}

void purge_list (doubll* list)
{
    doubll_item* temp;

    if (invalid_list(list)) return;

    temp = (list -> head).next;

    while (temp != NULL && temp -> next != NULL) {
        temp = remove_item(list, temp);
    }
}

void sort_list (doubll* list, list_less_func* less)
{
    doubll_item *temp, *insert_pos;

    if (invalid_list(list)) return;

    if ((list -> head).next -> next == NULL) return;

    temp = (list -> head).next -> next;

    while (temp -> next != NULL) {
        insert_pos = temp -> prev;

        while (insert_pos -> prev != NULL) {
            if (temp -> data != NULL && insert_pos != NULL && !(*less)(temp -> data, insert_pos -> data)) break;
            insert_pos = insert_pos -> prev;
        }

        move_item(insert_pos, temp);

        temp = temp -> next;
    }
}
