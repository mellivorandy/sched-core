// SPDX-License-Identifier: MIT
/**
 * @file rts_types.h
 * @brief Data types and structures.
 */
#ifndef RTS_TYPES_H
#define RTS_TYPES_H

#include "rts_list.h"

#include <stdio.h>

/**
 * struct rts_task - static attributes of a periodic task
 * @tid:       	  task id
 * @phase:     	  initial release offset
 * @period:    	  period of task
 * @rel_deadline: relative deadline
 * @wcet:      	  worst-case execution time
 * @util:      	  utilization (wcet/period)
 */
struct rts_task {
	int tid;
	int phase;
	int period;
	int rel_deadline;
	int wcet;
	double util;
	int release_count;
};

/**
 * struct rts_job - dynamic instance of a task
 * @tid:           parent task id
 * @jid:           job index (per-task)
 * @release:       release time
 * @abs_deadline:  absolute deadline
 * @remain:    	   remaining execution time
 * @qnode:         embedded list node for ready queue
 */
struct rts_job {
	int tid;
	int jid;
	
	int release_time;
	int abs_deadline;
	
	int remain;

	struct rts_list_head qnode;
};

/**
 * struct rts_sim - global simulation context
 * @n_tasks:    total number of tasks
 * @ready:      ready queue
 * @running:    currently running job
 * @clock:      current simulation time
 * @miss_counts: number of deadline misses
 * @job_counts:  number of jobs entered
 * @trace_fp:   trace output file pointer
 */
struct rts_sim {
	int clock;
	int n_tasks;
	int total_jobs;
	int missed_jobs;

	struct rts_task *tasks;
	struct rts_job *running;
	struct rts_list_head ready_queue;

	FILE *trace_fp;
};

#endif /* RTS_TYPES_H */
