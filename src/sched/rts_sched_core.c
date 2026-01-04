// SPDX-License-Identifier: MIT
/**
 * @file rts_sched_core.c
 * @brief Common helper functions for all scheduling classes.
 */
#include "rts_sched.h"
#include "rts_types.h"
#include "rts_rq.h"

#include <stdio.h>
#include <string.h>

/**
 * rts_sched_from_name - find scheduler class by name
 * @name: user-specified scheduler name
 */
extern const struct rts_sched_class rts_sched_rm;
extern const struct rts_sched_class rts_sched_edf;
extern const struct rts_sched_class rts_sched_lst;

/**
 * Scheduler Registry Table
 * @NULL: terminated array for easy iteration
 */
static const struct rts_sched_class * const available_schedulers[] = {
    &rts_sched_rm,
    &rts_sched_edf,
    &rts_sched_lst,
    // &rts_sched_rr,
    NULL
};

const struct rts_sched_class *rts_sched_from_name(const char *name) {
    if (!name || !*name) {
        fprintf(stderr, "Error: scheduler name is empty\n");
        fprintf(stderr, "Available: ");
        for (size_t i = 0; available_schedulers[i] != NULL; i++) {
            fprintf(stderr, "%s ", available_schedulers[i]->name);
        }
        fprintf(stderr, "\n");
        return NULL;
    }
	
	for (size_t i = 0; available_schedulers[i] != NULL; i++) {
        if (strcmp(name, available_schedulers[i]->name) == 0) {
            return available_schedulers[i];
        }
    }

    fprintf(stderr, "Error: Unknown scheduler '%s'\n", name);
    fprintf(stderr, "Available: ");
    for (size_t i = 0; available_schedulers[i] != NULL; i++) {
        fprintf(stderr, "%s ", available_schedulers[i]->name);
    }
    fprintf(stderr, "\n");
    
    return NULL;
}

/**
 * rts_sched_default_enqueue - generic ordered enqueue
 */
void rts_sched_default_enqueue(struct rts_sim *sim,
                               struct rts_job *job,
                               const struct rts_sched_class *sched) {
	rts_rq_ordered_insert(&sim->ready_queue, job, sched, sim->tasks, sim->clock);
}
