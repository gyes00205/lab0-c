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
    /* TODO: What if malloc returned NULL? */
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
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q)
        return;

    for (list_ele_t *cur = q->head; cur;) {
        list_ele_t *tmp = cur;
        cur = cur->next;
        free(tmp->value);
        free(tmp);
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
    list_ele_t *newh;
    int len = strlen(s);
    /* TODO: What should you do if the q is NULL? */
    if (!q)
        return false;

    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = malloc(sizeof(char) * (len + 1));
    if (!newh->value) {
        free(newh->value);
        free(newh);
        return false;
    }
    memset(newh->value, 0, len + 1);
    strncpy(newh->value, s, len);
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (q->size == 0)
        q->tail = newh;
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
    /* TODO: Remove the above comment when you are about to implement. */
    list_ele_t *newt;
    int len = strlen(s);
    if (!q)
        return false;

    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;

    newt->value = malloc(sizeof(char) * (len + 1));
    if (!newt->value) {
        free(newt->value);
        free(newt);
        return false;
    }
    memset(newt->value, 0, len + 1);
    strncpy(newt->value, s, len);
    newt->next = NULL;
    if (q->size == 0) {
        q->head = newt;
        q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }
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
    list_ele_t *tmp;
    int len;
    if (!q || q->size == 0)
        return false;

    tmp = q->head;
    len = strlen(tmp->value);
    if (sp) {
        memset(sp, 0, bufsize);
        (bufsize > len) ? strncpy(sp, tmp->value, len)
                        : strncpy(sp, tmp->value, bufsize - 1);
    }

    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return (!q) ? 0 : q->size;
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
    list_ele_t *cur, *tmp;

    if (!q || q->size == 0)
        return;

    q->tail = q->head;
    cur = q->head->next;
    q->tail->next = NULL;

    while (cur) {
        tmp = cur->next;
        cur->next = q->head;
        q->head = cur;
        cur = tmp;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t **indirect, *result = NULL;
    indirect = &result;

    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) <= 0) {
            *indirect = l1;
            l1 = l1->next;
        } else {
            *indirect = l2;
            l2 = l2->next;
        }
        indirect = &(*indirect)->next;
    }

    if (l1)
        *indirect = l1;
    if (l2)
        *indirect = l2;
    return result;
}

list_ele_t *mergeSortList(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    // split list
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    // sort each list
    list_ele_t *l1 = mergeSortList(head);
    list_ele_t *l2 = mergeSortList(fast);

    // merge sorted l1 and sorted l2
    return merge(l1, l2);
}

void q_sort(queue_t *q)
{
    if (!q || q->size == 0)
        return;
    q->head = mergeSortList(q->head);
    while (q->tail->next)
        q->tail = q->tail->next;
}
