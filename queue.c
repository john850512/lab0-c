#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }
    list_ele_t *next_ptr;
    while (q->head) {
        next_ptr = q->head->next;
        free(q->head->value);
        free(q->head);
        q->head = next_ptr;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    size_t str_len = strlen(s) + 1;
    newh->value = malloc(sizeof(char) * str_len);
    if (!newh->value) {
        free(newh);
        return false;
    }
    memcpy(newh->value, s, str_len);
    if (!q->head) {
        q->tail = newh;
    }
    newh->next = q->head;
    q->head = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q) {
        return false;
    }
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }
    size_t str_len = strlen(s) + 1;
    newt->value = malloc(sizeof(char) * str_len);
    if (!newt->value) {
        free(newt);
        return false;
    }
    memcpy(newt->value, s, str_len);
    newt->next = NULL;
    if (!q->tail) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->size) {
        return false;
    }
    if (sp) {
        memcpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_ele_t *temp = q->head;
    q->head = q->head->next;
    q->size--;
    free(temp->value);
    free(temp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{

    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size <= 1) {
        return;
    }
    list_ele_t *prev = NULL;
    list_ele_t *curr = q->head;
    list_ele_t *temp;
    while (curr) {
        temp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = temp;
    }
    /* swap the head & tail */
    temp = q->tail;
    q->tail = q->head;
    q->head = temp;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size < 2) {
        return;
    }
    q->head = merge_sort(q->head);
    /* move tail to the end of the list*/
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}

list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }
    /* find the mid of the list, and split it */
    list_ele_t *walk_slow = head;
    list_ele_t *walk_fast = head->next;
    while (walk_fast->next && walk_fast->next->next) {
        walk_slow = walk_slow->next;
        walk_fast = walk_fast->next->next;
    }
    walk_fast = walk_slow->next;
    walk_slow->next = NULL;

    list_ele_t *left = merge_sort(head);
    list_ele_t *right = merge_sort(walk_fast);
    return merge(left, right);
}

list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    if (!left) {
        return right;
    }
    if (!right) {
        return left;
    }
    /* find the min node of two list*/
    list_ele_t *new_head, *ptr;
    if (strcmp(left->value, right->value) < 0) {
        new_head = left;
        left = left->next;
    } else {
        new_head = right;
        right = right->next;
    }
    /* merge when both are no NULL*/
    ptr = new_head;
    while (left && right) {
        if (strcmp(left->value, right->value) < 0) {
            ptr->next = left;
            left = left->next;
        } else {
            ptr->next = right;
            right = right->next;
        }
        ptr = ptr->next;
    }
    /* check if exist remained node in the list*/
    if (left) {
        ptr->next = left;
    }
    if (right) {
        ptr->next = right;
    }
    return new_head;
}