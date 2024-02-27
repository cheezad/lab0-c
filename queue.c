#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


void q_merge_two(struct list_head *first,
                 struct list_head *second,
                 bool descend);


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

    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    new->value = strdup(s);
    if (!new->value) {
        free(new);
        return false;
    }
    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    new->value = strdup(s);
    if (!new->value) {
        free(new);
        return false;
    }
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
    if (!head || head->next == head)
        return false;
    struct list_head *slow, *fast;
    slow = fast = head->next;
    for (; fast != head && fast->next != head; fast = fast->next->next)
        slow = slow->next;
    element_t *temp = list_entry(slow, element_t, list);
    list_del(&temp->list);
    q_release_element(temp);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;
    if (list_empty(head) || list_is_singular(head))
        return true;

    struct list_head *first = head->next, *second = head->next->next;
    bool first_is_dup = false;
    while (first != head && second != head) {
        element_t *first_ent = list_entry(first, element_t, list),
                  *second_ent = list_entry(second, element_t, list);
        if (!strcmp(first_ent->value, second_ent->value)) {
            list_del(second);
            q_release_element(second_ent);
            second = first->next;
            first_is_dup = true;
        } else {
            if (first_is_dup) {
                first_is_dup = false;
                second = second->next;
                list_del(first);
                q_release_element(first_ent);
                first = second->prev;
            } else {
                first = first->next;
                second = second->next;
            }
        }
    }
    if (first_is_dup == true) {
        element_t *temp = list_entry(first, element_t, list);
        list_del(first);
        q_release_element(temp);
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;

    struct list_head *temp;
    list_for_each (temp, head) {
        if (temp->next == head)
            break;
        list_move(temp, temp->next);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || head->next == head)
        return;

    struct list_head *node = head;
    do {
        struct list_head *temp = node->next;
        node->next = node->prev;
        node->prev = temp;
        node = temp;
    } while (node != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || head->next == head || k <= 1)
        return;
    size_t count = 0;
    struct list_head *cur = NULL, *safe = NULL, *start = head;
    list_for_each_safe (cur, safe, head) {
        count++;
        if (count == k) {
            LIST_HEAD(temp);
            count = 0;

            list_cut_position(&temp, start, cur);
            q_reverse(&temp);
            list_splice(&temp, start);

            start = safe->prev;
        }
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || head->next == head || head->prev == head->next)
        return;
    struct list_head *slow = head, *fast = head->next;
    for (; fast != head && fast->next != head; fast = fast->next->next)
        slow = slow->next;
    struct list_head left;
    list_cut_position(&left, head, slow);
    q_sort(&left, descend);
    q_sort(head, descend);
    q_merge_two(head, &left, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || head->next == head)
        return 0;

    struct list_head *curr = head->prev;
    char *curr_min = list_entry(curr, element_t, list)->value;
    int i = 0;
    while (curr != head) {
        element_t *temp = list_entry(curr, element_t, list);
        if (strcmp(temp->value, curr_min) <= 0) {
            curr_min = temp->value;
            curr = curr->prev;
            i++;
            continue;
        }
        struct list_head *next_node = curr->prev;
        list_del(curr);
        free(temp->value);
        free(temp);
        curr = next_node;
    }
    return i;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || head->next == head)
        return 0;

    struct list_head *curr = head->prev;
    char *curr_max = list_entry(curr, element_t, list)->value;
    int i = 0;
    while (curr != head) {
        element_t *temp = list_entry(curr, element_t, list);
        if (strcmp(temp->value, curr_max) >= 0) {
            curr_max = temp->value;
            curr = curr->prev;
            i++;
            continue;
        }
        struct list_head *next_node = curr->prev;
        list_del(curr);
        free(temp->value);
        free(temp);
        curr = next_node;
    }
    return i;
}

void q_merge_two(struct list_head *first,
                 struct list_head *second,
                 bool descend)
{
    if (!first || !second)
        return;

    struct list_head temp;
    INIT_LIST_HEAD(&temp);
    while (!list_empty(first) && !list_empty(second)) {
        element_t *first_top = list_first_entry(first, element_t, list);
        element_t *second_top = list_first_entry(second, element_t, list);
        char *first_str = first_top->value, *second_str = second_top->value;
        bool checker;
        if (descend)
            checker = strcmp(first_str, second_str) > 0;
        else
            checker = strcmp(first_str, second_str) < 0;
        element_t *add_first = checker ? first_top : second_top;
        list_move_tail(&add_first->list, &temp);
    }
    list_splice_tail_init(first, &temp);
    list_splice_tail_init(second, &temp);
    list_splice(&temp, first);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || head->next == head)
        return 0;

    queue_contex_t *first = list_first_entry(head, queue_contex_t, chain);
    int size = q_size(first->q);
    if (list_is_singular(head))
        return size;
    queue_contex_t *second =
        list_entry(first->chain.next, queue_contex_t, chain);
    queue_contex_t *end = NULL;
    while (second != end) {
        size += q_size(second->q);
        q_sort(first->q, descend);
        q_sort(second->q, descend);
        q_merge_two(first->q, second->q, descend);
        if (!end)
            end = second;
        list_move_tail(&second->chain, head);
        second = list_entry(first->chain.next, queue_contex_t, chain);
    }
    return size;
}
