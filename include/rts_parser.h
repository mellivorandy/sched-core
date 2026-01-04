// SPDX-License-Identifier: MIT
/**
 * @file rts_parser.h
 * @brief Task set parser interface.
 *
 * Defines the public functions for loading task sets from files.
 */

#ifndef RTS_PARSER_H
#define RTS_PARSER_H

#include "rts_types.h"

/**
 * rts_parser_load_tasks - load task set from file
 * @path: file path
 * @n_tasks_out: output number of tasks
 *
 * Each line format: phase period deadline wcet
 */
struct rts_task *rts_parser_load_tasks(const char *path, int *n_tasks_out);

#endif /* RTS_PARSER_H */
