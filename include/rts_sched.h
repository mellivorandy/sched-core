// SPDX-License-Identifier: MIT
/**
 * @file rts_sched.h
 * @brief Scheduler interface
 *
 * Defines the public functions for manipulating the scheduler.
 */
#ifndef RTS_SCHED_H
#define RTS_SCHED_H

struct rts_sim;
struct rts_job;
struct rts_task;

/**
 * struct rts_sched_class - scheduler strategy interface
 * @name:   scheduler name (e.g., "RM", "EDF")
 * @higher_prio: return 1 if job a has higher priority than b
 * @enqueue: insert job into ready queue (default: ordered insert)
 * @tick:    optional tick handler (for RR)
 * @schedulability_test: optional static test before simulation
 */
struct rts_sched_class {
	const char *name;
	int (*higher_prio)(const struct rts_job *a,
	                   const struct rts_job *b,
	                   const struct rts_task *tasks,
	                   int now);
	void (*enqueue)(struct rts_sim *sim, struct rts_job *job);
	void (*tick)(struct rts_sim *sim);
	int (*schedulability_test)(const struct rts_task *tasks, int n_tasks);
};

/**
 * Factory method to look up a scheduler by name.
 */
const struct rts_sched_class *rts_sched_from_name(const char *name);

void rts_sched_default_enqueue(struct rts_sim *sim,
                               struct rts_job *job,
                               const struct rts_sched_class *sched);

#endif /* RTS_SCHED_H */
