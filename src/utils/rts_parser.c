// SPDX-License-Identifier: MIT
/**
 * @file rts_parser.c
 * @brief Task set parser implementation.
 */
#include "rts_parser.h"
#include "rts_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * rts_parser_load_tasks - read task set from text file
 * Format: phase period deadline wcet
 * 
 * Task ID (tid) is assigned by the parser in load order
 */
struct rts_task *rts_parser_load_tasks(const char *path, int *n_tasks_out) {
	FILE *fp = fopen(path, "r");
	
	if (!fp) {
		perror("open task file");
		return NULL;
	}

	int cap = 16;
	struct rts_task *tasks = malloc(sizeof(*tasks) * cap);
	int count = 0;

	if (!tasks) {
    	fclose(fp);
    	return NULL;
	}

	char line[256];

	while (fgets(line, sizeof(line), fp)) {
		if (line[0] == '#' || line[0] == '\n')
			continue;

		for (char *p = line; *p; ++p) {
			if (*p == ',') {
				*p = ' ';
			}
		}

		if (count == cap) {
        	cap *= 2;
        	struct rts_task *tmp = realloc(tasks, sizeof(*tasks) * cap);
        	
			if (!tmp) {
            	free(tasks);
            	fclose(fp);
            	return NULL;
        	}

        	tasks = tmp;
    	}
		
		int phase, period, deadline, wcet;
		if (sscanf(line, "%d %d %d %d",
				   &phase, &period, &deadline, &wcet) == 4) {
			
			if (period <= 0 || wcet <= 0)
        		continue;
			
			tasks[count].tid = count;
			tasks[count].phase = phase;
			tasks[count].period = period;
			tasks[count].rel_deadline = deadline;
			tasks[count].wcet = wcet;
			tasks[count].util = (double)wcet / period;
			count++;
		}
	}

	fclose(fp);
	*n_tasks_out = count;
	return tasks;
}
