#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *q = malloc(sizeof(struct list_head));
    if (!q)
        return NULL;

    INIT_LIST_HEAD(q);
    return q;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;

    element_t *node, *temp;
    list_for_each_entry_safe (node, temp, l, list) {
        if (node->value)
            free(node->value);
        free(node);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = (element_t *) malloc(sizeof(*new));
    if (!new)
        return false;
    int slen = strlen(s);
    new->value = (char *) malloc(slen + 1);
    if (!new->value) {
        free(new);
        return false;
    }

    strncpy(new->value, s, slen);
    new->value[slen] = '\0';
    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = (element_t *) malloc(sizeof(*new));
    if (!new)
        return false;
    int slen = strlen(s);
    new->value = (char *) malloc(slen + 1);
    if (!new->value) {
        free(new);
        return false;
    }

    strncpy(new->value, s, slen);
    new->value[slen] = '\0';
    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || head == head->next)
        return NULL;
    element_t *temp = list_first_entry(head, element_t, list);
    if (sp && temp->value) {
        strncpy(sp, temp->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_del(head->next);
    return temp;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || head == head->next)
        return NULL;
    element_t *temp = list_last_entry(head, element_t, list);
    if (sp && temp->value) {
        strncpy(sp, temp->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_del(head->prev);
    return temp;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int count = 0;
    struct list_head *node;
    list_for_each (node, head)
        count++;
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (list_empty(head) || list_is_singular(head))
        return false;
    struct list_head **indir = &head;
    for (struct list_head *fast = (head)->next;
         fast != head && fast->next != head; fast = fast->next->next)
        indir = &(*indir)->next;
    element_t *temp = list_entry(*indir, element_t, list);
    *indir = (*indir)->next;
    if (temp->value)
        free(temp->value);
    free(temp);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
