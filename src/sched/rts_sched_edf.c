// SPDX-License-Identifier: MIT
/**
 * @file rts_sched_edf.c
 * @brief Earliest Deadline First (EDF) scheduling class.
 */
#include "rts_sched.h"
#include "rts_types.h"
#include "rts_util.h"

#include <stdio.h>

/**
 * edf_higher_prio - compare priority of two jobs
 * @a: job A
 * @b: job B
 * @tasks: task set
 * @now: current time
 *
 * @return true if A has higher priority than B.
 */
static int edf_higher_prio(const struct rts_job *a,
                           const struct rts_job *b,
                           const struct rts_task *tasks,
                           int now) {
	(void)tasks;
	(void)now;

	// Tie-breaker: lower TID
	if (a->abs_deadline == b->abs_deadline)
		return a->tid < b->tid;
	
	return a->abs_deadline < b->abs_deadline;
}

static int edf_schedulability_test(const struct rts_task *tasks, int n) {
	double U = 0.0;

	for (int i = 0; i < n; i++) {
		int denom = rts_min_int(tasks[i].period, tasks[i].rel_deadline);
		U += (double)tasks[i].wcet / denom;
	}
	printf("[EDF] U=%.3f → %s\n", U, (U <= 1.0) ? "Schedulable" : "Unschedulable");
	
	return U <= 1.0;
}

/* Scheduler class */
const struct rts_sched_class rts_sched_edf = {
    .name = "EDF",
    .higher_prio = edf_higher_prio,
    .enqueue = NULL,
    .tick = NULL,
    .schedulability_test = edf_schedulability_test,
};
