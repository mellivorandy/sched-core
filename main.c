// SPDX-License-Identifier: MIT
/**
 * @file main.c
 * @brief Entry point for RTOS scheduling simulator (rtsim).
 */
#include "rts_list.h"
#include "rts_parser.h"
#include "rts_sched.h"
#include "rts_trace.h"
#include "rts_types.h"
#include "rts_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rts_rq_init(struct rts_list_head *head);

void rts_sim_run(struct rts_sim *sim,
                 const struct rts_sched_class *sched,
                 int lcm, int max_phase);

/**
 * main - entry point
 * @argc: argument count
 * @argv: [1] scheduler name, [2] task file
 *
 * Example:
 *     ./rtsim EDF task.txt
 */
int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <SCHED> <task.txt>\n", argv[0]);
		fprintf(stderr, "Example: %s EDF task.txt\n", argv[0]);
		return 1;
	}

	const char *sched_name = argv[1];
	const char *task_file = argv[2];

	int n_tasks = 0;
	struct rts_task *tasks = rts_parser_load_tasks(task_file, &n_tasks);
	if (!tasks) {
		fprintf(stderr, "Failed to load %s\n", task_file);
		return 1;
	}

	/* Compute hyperperiod and max phase */
	int *periods = malloc(sizeof(int) * n_tasks);
	int max_phase = 0;
	for (int i = 0; i < n_tasks; i++) {
		periods[i] = tasks[i].period;

		if (tasks[i].phase > max_phase)
			max_phase = tasks[i].phase;
	}
	int lcm = rts_util_hyperperiod(periods, n_tasks);
	free(periods);

	printf("Loaded %d tasks. LCM=%d, MaxPhase=%d\n", n_tasks, lcm, max_phase);

	const struct rts_sched_class *sched = rts_sched_from_name(sched_name);
	if (!sched)
		return 1;

	/* Schedulability test */
	if (sched->schedulability_test) {
		int ok = sched->schedulability_test(tasks, n_tasks);
		
		if (!ok)
			printf("[Warn] Task set may miss deadlines under %s policy.\n", sched->name);
	}

	char outpath[512];
	FILE *trace = rts_trace_open("output", task_file, sched_name, outpath, sizeof(outpath));
	if (trace) {
		printf("[trace] writing to %s\n\n", outpath);
	}

	/* Simulation context */
	struct rts_sim sim = {
	    .tasks = tasks,
	    .n_tasks = n_tasks,
	    .clock = 0,
	    .missed_jobs = 0,
	    .total_jobs = 0,
	    .trace_fp = trace,
	};
	rts_rq_init(&sim.ready_queue);
	sim.running = NULL;

	/* Run simulation */
	rts_sim_run(&sim, sched, lcm, max_phase);

	printf("Simulation complete. Misses=%d, Jobs=%d\n",
	       sim.missed_jobs, sim.total_jobs);

	if (trace) {
		fclose(trace);
		sim.trace_fp = NULL;
	}

	free(tasks);
	return 0;
}
