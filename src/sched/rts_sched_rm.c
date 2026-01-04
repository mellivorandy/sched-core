// SPDX-License-Identifier: MIT
/**
 * @file rts_sched_rm.c
 * @brief Implementation of Rate Monotonic (RM) scheduling policy.
 */
#include "rts_sched.h"
#include "rts_types.h"
#include "rts_util.h"

#include <math.h>
#include <stdio.h>

static int rm_higher_prio(const struct rts_job *a,
                          const struct rts_job *b,
                          const struct rts_task *tasks,
                          int now) {
	(void)now;

	// Tie-breaker: lower TID
	if (tasks[a->tid].period == tasks[b->tid].period)
		return a->tid < b->tid;

	return tasks[a->tid].period < tasks[b->tid].period;
}

static int rm_schedulability_test(const struct rts_task *tasks, int n) {
	double U = 0.0;

	for (int i = 0; i < n; i++) {
		int denom = rts_min_int(tasks[i].period, tasks[i].rel_deadline);
		U += (double)tasks[i].wcet / denom;
	}
	double bound = n * (pow(2.0, 1.0 / n) - 1.0);
	printf("[RM] U=%.3f bound=%.3f → %s\n", U, bound, (U <= bound) ? "Schedulable" : "Unschedulable");
	
	return U <= bound;
}

const struct rts_sched_class rts_sched_rm = {
    .name = "RM",
    .higher_prio = rm_higher_prio,
    .enqueue = NULL,
    .tick = NULL,
    .schedulability_test = rm_schedulability_test,
};
