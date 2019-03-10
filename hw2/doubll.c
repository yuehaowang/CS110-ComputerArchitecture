#include "doubll.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*********************** Helpers ***********************/

/** used to move temp after prev */
static void move_item (doubll_item* prev, doubll_item* temp)
{
    /** set prev's next to temp's next */
    if (temp -> prev != NULL) temp -> prev -> next = temp -> next;
    /** set next's prev to temp's prev */
    if (temp -> next != NULL) temp -> next -> prev = temp -> prev;

    /** set temp's prev to prev */
    temp -> prev = prev;
    /** set temp's next to prev's next */
    temp -> next = prev -> next;
    /** set prev of prev's next to temp */
    if (prev -> next != NULL) prev -> next -> prev = temp;
    /** set prev's next to temp */
    prev -> next = temp;
}

#ifdef CHECK_LIST
/** used to check whether the item is in the list */
static bool is_item_in_list (doubll* list, doubll_item* item)
{
    doubll_item* temp;
    
    temp = &(list -> head);
    /** visit each item in the list recursively */
    while (temp != NULL) {
        if (temp == item) return true;
        temp = temp -> next;
    }

    return false;
}
#endif

/** a validation function to check these requirements:
    1. prev of head must be NULL
    2. next of head cannot be NULL
    3. prev of tail cannot be NULL
    4. next of tail must be NULL
*/
static bool invalid_list (doubll* list)
{
    return (list == NULL) || (list -> head).prev != NULL || (list -> head).next == NULL || (list -> tail).next != NULL || (list -> tail).prev == NULL;
}


/*********************** Implementation ***********************/

void list_init (doubll *list)
{
    /** init head */
    (list -> head).next = &(list -> tail);
    (list -> head).prev = NULL;
    (list -> head).data = NULL;
    (list -> head).size = 0;
    /** init tail */
    (list -> tail).prev = &(list -> head);
    (list -> tail).next = NULL;
    (list -> tail).data = NULL;
    (list -> tail).size = 0;
    /** init items */
    list -> items = 0;
}

doubll_item* list_begin (doubll* list)
{
    /** check the validation of list to give the begin item without potential errors */
    if (invalid_list(list)) return NULL;
    return (list -> head).next;
}

doubll_item* list_head (doubll* list)
{
    /** check the validation of list to return an available head */
    if (invalid_list(list)) return NULL;
    return &(list -> head);
}

doubll_item* list_end (doubll* list)
{
    /** check the validation of list to make sure the tail item is available */
    if (invalid_list(list)) return NULL;
    return &(list -> tail);
}

size_t list_size (doubll* list)
{
    /** check the validation of list */
    if (invalid_list(list)) return -1;
    return list -> items;
}

doubll_item* insert_list (doubll* list, doubll_item* item, void* data, size_t size)
{
    doubll_item* new_item;

    /** check the validation of list, item, data and size */
    if (invalid_list(list) || item == NULL || item -> next == NULL || data == NULL || size == 0) return NULL;

    /** if CHECK_LIST is defined, check whether the item is in the list */
    #ifdef CHECK_LIST
    if (!is_item_in_list(list, item)) return NULL;
    #endif

    /** allocate a memory for the new item */
    new_item = (doubll_item*)malloc(sizeof(doubll_item));
    if (new_item == NULL) return NULL;
    /** copy the data to the new item's data */
    new_item -> data = (void*)malloc(size);
    if (new_item -> data == NULL) return NULL;
    memcpy(new_item -> data, data, size);
    new_item -> size = size;

    /** set the prev and next of the new item */
    new_item -> prev = item;
    new_item -> next = item -> next;
    /** update the parent item */
    item -> next -> prev = new_item;
    item -> next = new_item;

    /** increase the num of the items */
    (list -> items)++;

    return new_item;
}

doubll_item* remove_item (doubll* list, doubll_item* item)
{
    doubll_item* next;

    if (invalid_list(list) || item == NULL || item -> prev == NULL || item -> next == NULL) return NULL;

    /** if CHECK_LIST is defined, check whether the item is in the list */
    #ifdef CHECK_LIST
    if (!is_item_in_list(list, item)) return NULL;
    #endif

    /** store the next item as the return value */
    next = item -> next;

    /** break the relations between the item and its neighbors */
    item -> prev -> next = item -> next;
    item -> next -> prev = item -> prev;

    /** free data */
    if(item -> data != NULL) free(item -> data);
    item -> data = NULL;
    /** free item itself */
    free(item);

    /** decrease the num of the items in the list */
    (list -> items)--;

    return next;
}

void purge_list (doubll* list)
{
    doubll_item* temp;

    if (invalid_list(list)) return;

    temp = (list -> head).next;

    /** visit each item and remove them from the list by recursion */
    while (temp != NULL && temp -> next != NULL) {
        temp = remove_item(list, temp);
    }
}

/** apply insertion sort */
void sort_list (doubll* list, list_less_func* less)
{
    doubll_item *temp, *insert_pos;

    if (invalid_list(list)) return;

    /** if the list only has head and tail, there is nothing to sort */
    if ((list -> head).next -> next == NULL) return;

    temp = (list -> head).next -> next;

    /** visit each item in the list and put them forward to an appropriate position */
    while (temp -> next != NULL) {
        insert_pos = temp -> prev;

        while (insert_pos -> prev != NULL) {
            /** compare items via less function */
            if (temp -> data != NULL && insert_pos != NULL && !(*less)(temp -> data, insert_pos -> data)) break;
            insert_pos = insert_pos -> prev;
        }

        /** move the item to the appropriate position */
        move_item(insert_pos, temp);

        temp = temp -> next;
    }
}
