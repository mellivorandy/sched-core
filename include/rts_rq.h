// SPDX-License-Identifier: MIT
/**
 * @file rts_rq.h
 * @brief Ready queue interface.
 *
 * Defines the public functions for manipulating the ready queue.
 */
#ifndef RTS_RQ_H
#define RTS_RQ_H

struct rts_list_head;
struct rts_job;
struct rts_task;
struct rts_sched_class;

void rts_rq_init(struct rts_list_head *head);

void rts_rq_enqueue(struct rts_list_head *rq, struct rts_job *job);

struct rts_job *rts_rq_dequeue(struct rts_list_head *rq);

void rts_rq_remove(struct rts_job *job);

/* Ordered insert according to scheduler policy */
void rts_rq_ordered_insert(struct rts_list_head *head,
                           struct rts_job *job,
                           const struct rts_sched_class *sched,
                           const struct rts_task *tasks,
                           int now);

/* Debug print */
void rts_rq_dump(struct rts_list_head *head, const struct rts_task *tasks);

#endif /* RTS_RQ_H */
