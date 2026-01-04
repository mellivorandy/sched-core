// SPDX-License-Identifier: MIT
/**
 * @file rts_rq_list.c
 * @brief Ready queue implementation using intrusive circular doubly linked list.
 *
 * This module provides basic ready queue operations.
 */
#include "rts_sched.h"
#include "rts_types.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * rts_rq_init - initialize ready queue
 * @head: list head
 */
void rts_rq_init(struct rts_list_head *head) {
	rts_list_init(head);
}

/**
 * rts_rq_enqueue_tail - enqueue a job at tail (FIFO order)
 * @head: ready queue head
 * @job: job to enqueue
 */
void rts_rq_enqueue(struct rts_list_head *head, struct rts_job *job) {
	rts_list_add_tail(&job->qnode, head);
}

/**
 * rts_rq_dequeue_head - dequeue the first job (FIFO)
 * @head: ready queue head
 * Return: pointer to job or NULL if empty
 */
struct rts_job *rts_rq_dequeue(struct rts_list_head *head) {
	if (rts_list_empty(head))
		return NULL;

	struct rts_list_head *node = head->next;
	rts_list_del(node);
	return rts_list_entry(node, struct rts_job, qnode);
}

/**
 * rts_rq_ordered_insert - insert job by scheduler priority rule
 * @head: ready queue head
 * @job: new job
 * @sched: scheduler class defining higher_prio()
 * @tasks: task set (for lookup)
 * @now: current time
 *
 * Iterate the queue and insert before the first lower-priority job.
 */
void rts_rq_ordered_insert(struct rts_list_head *head,
                           struct rts_job *job,
                           const struct rts_sched_class *sched,
                           const struct rts_task *tasks,
                           int now) {
	struct rts_list_head *pos;
	for (pos = head->next; pos != head; pos = pos->next) {
		struct rts_job *cur = rts_list_entry(pos, struct rts_job, qnode);
		if (sched->higher_prio(job, cur, tasks, now)) {
			/* insert before cur */
			rts_list_link_between(&job->qnode, pos->prev, pos);
			return;
		}
	}
	/* if all have higher priority, append at tail */
	rts_rq_enqueue(head, job);
}

/**
 * rts_rq_remove_job - remove a specific job from queue
 * @job: job to remove
 */
void rts_rq_remove(struct rts_job *job) {
	rts_list_del(&job->qnode);
}

/**
 * rts_rq_dump - dump queue content
 * @head: ready queue head
 * @tasks: task set (for metadata)
 */
void rts_rq_dump(struct rts_list_head *head, const struct rts_task *tasks) {
	(void)tasks;

	struct rts_list_head *pos;
	printf("[RQ] ");
	for (pos = head->next; pos != head; pos = pos->next) {
		struct rts_job *j = rts_list_entry(pos, struct rts_job, qnode);
		printf("T%d:J%d(rem=%d,d=%d) -> ",
		       j->tid + 1, j->jid, j->remain, j->abs_deadline);
	}
	printf("END\n\n");
}
