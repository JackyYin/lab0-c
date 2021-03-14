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
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    while (q->head) {
        list_ele_t *cur = q->head;
        q->head = q->head->next;
        free(cur->value);
        free(cur);
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
    if (!(newh = malloc(sizeof(list_ele_t)))) {
        return false;
    }
    if (!s) {
        newh->value = NULL;
    } else {
        if (!(newh->value = malloc(strlen(s) + 1))) {
            free(newh);
            return false;
        }
        strncpy(newh->value, s, strlen(s) + 1);
    }
    newh->next = q->head;
    q->head = newh;
    if (++q->size == 1) {
        q->tail = newh;
    }
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
    list_ele_t *newt;
    if (!(newt = malloc(sizeof(list_ele_t)))) {
        return false;
    }
    if (!s) {
        newt->value = NULL;
    } else {
        if (!(newt->value = malloc(strlen(s) + 1))) {
            free(newt);
            return false;
        }
        strncpy(newt->value, s, strlen(s) + 1);
    }
    newt->next = NULL;
    if (q->tail) {
        q->tail->next = newt;
        q->tail = newt;
    } else {
        q->tail = newt;
        q->head = newt;
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
    if (!q || !q->size)
        return false;

    list_ele_t *next = q->head->next;
    char *str = q->head->value;
    free(q->head);
    if (sp) {
        memcpy(sp, str, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    free(str);

    q->head = next;

    if (q->size-- == 1) {
        q->tail = next;
    }
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
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
    if (!q || q->size < 2)
        return;

    list_ele_t *tmp = q->head->next;
    q->tail->next = q->head;

    while (tmp != q->tail) {
        tmp = tmp->next;

        q->head->next->next = q->tail->next;
        q->tail->next = q->head->next;
        q->head->next = tmp;
    }

    q->tail = q->head;
    q->tail->next = NULL;
    q->head = tmp;
}

static inline list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head->next)
        return head;

    list_ele_t *slow = head;
    list_ele_t *fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    fast = slow->next;
    slow->next = NULL;

    slow = merge_sort(head);
    fast = merge_sort(fast);

    list_ele_t *cur = NULL;
    list_ele_t **indirect = &cur;

    while (slow && fast) {
        if (strcmp(slow->value, fast->value) >= 0) {
            *indirect = slow;
            slow = slow->next;
        } else {
            *indirect = fast;
            fast = fast->next;
        }
        indirect = &((*indirect)->next);
    }

    if (slow)
        *indirect = slow;
    if (fast)
        *indirect = fast;

    return cur;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size < 2)
        return;

    q->head = merge_sort(q->head);

    while (q->tail && q->tail->next) {
        q->tail = q->tail->next;
    }
}
