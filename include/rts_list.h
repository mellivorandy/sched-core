// SPDX-License-Identifier: MIT
/**
 * @file rts_list.h
 * @brief Intrusive circular doubly linked list implementation.
 */
#ifndef RTS_LIST_H
#define RTS_LIST_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * struct rts_list_head
 * @brief Intrusive list node / sentinel head.
 */
struct rts_list_head {
	struct rts_list_head *next, *prev;
};

#define RTS_LIST_HEAD_INIT(name) {&(name), &(name)}
#define RTS_LIST_HEAD(name) struct rts_list_head name = RTS_LIST_HEAD_INIT(name)

static inline void rts_list_init(struct rts_list_head *list) {
	list->next = list;
	list->prev = list;
}

static inline int rts_list_empty(const struct rts_list_head *head) {
	return head->next == head;
}

static inline void rts_list_link_between(struct rts_list_head *node,
                                        struct rts_list_head *before,
                                        struct rts_list_head *after) {
	node->prev = before;
	node->next = after;
	before->next = node;
	after->prev = node;
}

/**
 * @brief Append a node to the end of a list.
 *
 * @param node Node to be inserted.
 * @param head Sentinel head of the list.
 */
static inline void rts_list_push_back(struct rts_list_head *node,
                                     struct rts_list_head *head) {
	rts_list_link_between(node, head->prev, head);
}

/**
 * @brief Remove a node from its list.
 */
static inline void rts_list_erase(struct rts_list_head *node) {
	struct rts_list_head *before = node->prev;
	struct rts_list_head *after  = node->next;

	before->next = after;
	after->prev = before;

	node->next = node;
	node->prev = node;
}

#define rts_list_container(node_ptr, type, member) \
	((type *)((char *)(node_ptr) - offsetof(type, member)))

#define rts_list_entry(node_ptr, type, member) rts_list_container(node_ptr, type, member)

#define rts_list_for_each(pos, head) \
	for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

/**
 * rts_list_for_each_safe
 * @brief Forward iteration safe for node deletion.
 * @param pos Cursor (struct rts_list_head *).
 * @param tmp Temp storage for next node (struct rts_list_head *).
 * @param head List head (sentinel).
 */
#define rts_list_for_each_safe(pos, tmp, head) \
    for ((pos) = (head)->next, (tmp) = (pos)->next; \
         (pos) != (head); \
         (pos) = (tmp), (tmp) = (pos)->next)

static inline void rts_list_add_tail(struct rts_list_head *n, struct rts_list_head *head) {
	rts_list_push_back(n, head);
}

static inline void rts_list_del(struct rts_list_head *entry) {
	rts_list_erase(entry);
}

static inline void rts_list_push_front(struct rts_list_head *node,
                                       struct rts_list_head *head) {
    rts_list_link_between(node, head, head->next);
}

static inline struct rts_list_head *
rts_list_pop_front(struct rts_list_head *head) {
    if (rts_list_empty(head))
        return NULL;

    struct rts_list_head *n = head->next;
    rts_list_erase(n);
    return n;
}

#ifdef __cplusplus
}
#endif

#endif /* RTS_LIST_H */
