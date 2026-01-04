// SPDX-License-Identifier: MIT
/**
 * @file rts_sim.c
 * @brief Main simulation loop for RTOS scheduling.
 */
#include "rts_rq.h"
#include "rts_sched.h"
#include "rts_types.h"
#include "rts_util.h"
#include "rts_log.h"

#include <stdio.h>
#include <stdlib.h>

static void rts_sim_cleanup(struct rts_sim *sim);

/**
 * rts_sim_run - main simulation loop
 * @sim: simulation context
 * @sched: chosen scheduler class
 * @lcm: total hyperperiod
 * @max_phase: max phase offset
 */
void rts_sim_run(struct rts_sim *sim,
                 const struct rts_sched_class *sched,
                 int lcm, int max_phase) {
	printf("Starting simulation with %s policy\n", sched->name);
	printf("Loaded %d tasks:\n", sim->n_tasks);

	for (int i = 0; i < sim->n_tasks; i++) {
		struct rts_task *t = &sim->tasks[i];
		printf("T%d: phase=%d, period=%d, deadline=%d, wcet=%d, util=%.2f\n",
		       t->tid + 1, t->phase, t->period, t->rel_deadline, t->wcet, t->util);
	}
	printf("\n--------------------------------------------\n\n");

	rts_rq_init(&sim->ready_queue);

	while (sim->clock <= (lcm + max_phase)) {
		// Check for deadline misses
		struct rts_list_head *p, *n;
		rts_list_for_each_safe(p, n, &sim->ready_queue) {
			struct rts_job *job = rts_list_entry(p, struct rts_job, qnode);

			if (job->remain <= 0) {
				continue;
			}

			// Deadline miss detected
			if (sim->clock >= job->abs_deadline) {
				RTS_LOG_MISS("T%d:J%d missed its deadline (t=%d, d=%d)\n",
				       job->tid + 1, job->jid, sim->clock, job->abs_deadline);
				
				sim->missed_jobs++;
				rts_rq_remove(job);
				free(job);
				continue;
			}

			// Not enough time to finish before deadline
			if (sim->clock + job->remain > job->abs_deadline) {
				RTS_LOG_MISS_IMMINENT("T%d:J%d cannot meet deadline (t=%d, d=%d, rem=%d)\n",
				       job->tid + 1, job->jid, sim->clock, job->abs_deadline, job->remain);
				
				sim->missed_jobs++;
				rts_rq_remove(job);
				free(job);
				continue;
			}

			// int slack = job->abs_dead - sim->clock - job->remain;
			// if (slack < 0) {
			// 	printf("[LST] Job T%d missed deadline (d=%d, now=%d, remain=%d, slack=%d)\n",
			// 	       job->tid, job->abs_dead, sim->clock, job->remain, slack);
			// 	sim->missed_jobs++;
			// 	rts_rq_remove(job);
			// 	free(job);
			// }
		}

		// Skip job arrivals and execution at the end time
		// no job arrivals or execution, only check deadline misses
		if (sim->clock == (lcm + max_phase)) {
			RTS_LOG_END("\n[%03d]\n", sim->clock);
			rts_rq_dump(&sim->ready_queue, sim->tasks);
			sim->clock++;
			continue;
		}

		// New job arrivals
		for (int i = 0; i < sim->n_tasks; i++) {
			struct rts_task *t = &sim->tasks[i];
			
			if ((sim->clock - t->phase) >= 0 &&
			    ((sim->clock - t->phase) % t->period) == 0) {

				struct rts_job *j = calloc(1, sizeof(*j));
				j->tid = t->tid;
				j->jid = ++t->release_count;
				j->remain = t->wcet;
				j->abs_deadline = sim->clock + t->rel_deadline;
				j->release_time = sim->clock;
				rts_list_init(&j->qnode);

				if (sched->enqueue) {
    				sched->enqueue(sim, j);
				} else {
    				rts_sched_default_enqueue(sim, j, sched);
				}

				sim->total_jobs++;

				RTS_LOG_ARRIVAL("T%d:J%d (release=%d, deadline=%d)\n",
				       j->tid + 1, j->jid, j->release_time, j->abs_deadline);
			}
		}

		// Select the job to run
		struct rts_job *cur = NULL;
		rts_list_for_each(p, &sim->ready_queue) {
			struct rts_job *j = rts_list_entry(p, struct rts_job, qnode);
			if (j->remain <= 0) {
				continue;
			}
			if (!cur || sched->higher_prio(j, cur, sim->tasks, sim->clock)) {
				cur = j;
			}
		}

		// Execute the selected job for one time unit
		if (cur) {
			cur->remain--;
			RTS_LOG_RUN(sim->clock,
            			"T%d:J%d (remain=%d)",
            			cur->tid + 1, cur->jid, cur->remain);

			if (sim->trace_fp) {
				fprintf(sim->trace_fp, "[%03d] T%d:J%d\n",
				        sim->clock, cur->tid + 1, cur->jid);
			}

			if (cur->remain == 0) {
				RTS_LOG_DONE("T%d:J%d finished at t=%d\n",
				       cur->tid + 1, cur->jid, sim->clock + 1);
				
				rts_rq_remove(cur);
				free(cur);
			}
		} else {
			RTS_LOG_RUN(sim->clock, "IDLE\n");
			fprintf(sim->trace_fp, "[%03d] IDLE\n", sim->clock);
		}

		rts_rq_dump(&sim->ready_queue, sim->tasks);

		sim->clock++;
	}

	rts_sim_cleanup(sim);

	printf("--------------------------------------------\n");
	printf("Simulation complete.\n");
	printf("Total jobs released: %d\n", sim->total_jobs);
	printf("Missed deadlines: %d\n", sim->missed_jobs);
}

static void rts_sim_cleanup(struct rts_sim *sim) {
    struct rts_list_head *p, *n;

    rts_list_for_each_safe(p, n, &sim->ready_queue) {
        struct rts_job *job = rts_list_entry(p, struct rts_job, qnode);
        rts_rq_remove(job);
        free(job);
    }
}
