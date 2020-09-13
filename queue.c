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
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Avoid freeing a empty queue. */
    if (!q)
        return;

    list_ele_t *ptr, *tmp;
    tmp = ptr = q->head;

    /* The queue might be empty */
    if (!ptr) {
        free(q);
        return;
    }

    while (ptr->next) {
        ptr = ptr->next;
        free(tmp->value);
        free(tmp);
        tmp = ptr;
    }
    free(tmp->value);
    free(tmp);

    /* Free queue structure */
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
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = malloc(strlen(s) + 1);
    /*
     * If fail to allocate the space of the string, free the new the node and
     * return false .
     */
    if (!newh->value) {
        free(newh);
        return false;
    }
    /* Copy the string into the new node */
    strncpy(newh->value, s, strlen(s));
    /* Append the null byte in the end of the string */
    newh->value[strlen(s)] = '\0';
    q->size++;

    /* If the queue is empty, store the new node address in the tail pointer. */
    if (!q->head)
        q->tail = newh;
    newh->next = q->head;
    q->head = newh;
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
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = malloc(strlen(s) + 1);
    /*
     * If fail to allocate the space of the string, free the new the node and
     * return false .
     */
    if (!newh->value) {
        free(newh);
        return false;
    }
    /* Copy the string into the new node */
    strncpy(newh->value, s, strlen(s));
    /* Append the null byte in the end of the string */
    newh->value[strlen(s)] = '\0';
    q->size++;

    newh->next = NULL;
    if (!q->tail) {
        q->head = newh;
        q->tail = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    return true;
}

size_t min(size_t num1, size_t num2)
{
    return num1 < num2 ? num1 : num2;
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
    if (!sp)
        return false;
    if (!q)
        return false;
    list_ele_t *tmp = q->head;
    if (!tmp)
        return false;
    if (!tmp->next) {
        q->head = NULL;
        q->tail = NULL;
    } else
        q->head = tmp->next;

    size_t len = min(bufsize - 1, strlen(tmp->value));
    /* Copy the string to sp */
    strncpy(sp, tmp->value, len);
    /* Append null byte */
    sp[len] = '\0';
    /* Free the node */
    q->size--;

    free(tmp->value);
    free(tmp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
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
    if (!q)
        return;
    list_ele_t *newh = NULL, *newt, *ptr = q->head;
    newt = q->head;
    while (ptr) {
        list_ele_t *tmp = ptr;
        ptr = ptr->next;
        tmp->next = newh;
        newh = tmp;
    }
    q->head = newh;
    q->tail = newt;
}

bool cmp(const char *str1, const char *str2)
{
    for (; *str1 && *str2; str1++, str2++) {
        if (*str1 < *str2)
            return true;
        else if (*str1 > *str2)
            return false;
    }
    if (*str1)
        return false;
    return true;
}

list_ele_t *merge(list_ele_t *list1, list_ele_t *list2)
{
    /* list might be NULL */
    if (!list1)
        return list2;
    if (!list2)
        return list1;

    list_ele_t *head = NULL, *tail = NULL;
    /* find the head of the list */
    list_ele_t *tmp;
    if (cmp(list1->value, list2->value)) {
        tmp = list1;
        list1 = list1->next;
    } else {
        tmp = list2;
        list2 = list2->next;
    }
    head = tail = tmp;

    while (list1 && list2) {
        if (cmp(list1->value, list2->value)) {
            tmp = list1;
            list1 = list1->next;
        } else {
            tmp = list2;
            list2 = list2->next;
        }
        tail->next = tmp;
        tail = tmp;
    }

    if (list1)
        tail->next = list1;
    if (list2)
        tail->next = list2;
    return head;
}

list_ele_t *mergeSort(list_ele_t *list, unsigned int size)
{
    if (size < 2)
        return list;

    unsigned int rsize = size / 2;
    unsigned int lsize = size - rsize;
    list_ele_t *rptr, *lptr;
    rptr = lptr = list;
    for (int i = 1; i < lsize; i++)
        rptr = rptr->next;
    list_ele_t *tmp = rptr;
    rptr = rptr->next;
    tmp->next = NULL;

    lptr = mergeSort(lptr, lsize);
    rptr = mergeSort(rptr, rsize);
    return merge(lptr, rptr);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;

    // return;
    q->head = mergeSort(q->head, q->size);
    list_ele_t *ptr = q->head;
    while (ptr->next)
        ptr = ptr->next;
    q->tail = ptr;
    printf("tail: %s\n", q->tail->value);
}
