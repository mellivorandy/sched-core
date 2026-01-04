// SPDX-License-Identifier: MIT
/**
 * @file rts_sched_lst.c
 * @brief Implementation of Least Slack Time (LST) scheduling policy.
 */
#include "rts_sched.h"
#include "rts_types.h"

#include <stdio.h>

static int lst_higher_prio(const struct rts_job *a,
                           const struct rts_job *b,
                           const struct rts_task *tasks,
                           int now) {
	(void)tasks;

	int slack_a = a->abs_deadline - now - a->remain;
	int slack_b = b->abs_deadline - now - b->remain;

	// Tie-breaker: lower TID
	if (slack_a == slack_b)
		return a->tid < b->tid;
	
	return slack_a < slack_b;
}

static int lst_schedulability_test(const struct rts_task *tasks, int n) {
	(void)tasks;
	(void)n;

	printf("[LST] No analytic schedulability test; run simulation.\n");
	return 1;
}

const struct rts_sched_class rts_sched_lst = {
    .name = "LST",
    .higher_prio = lst_higher_prio,
    .enqueue = NULL,
    .tick = NULL,
    .schedulability_test = lst_schedulability_test,
};
