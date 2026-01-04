// SPDX-License-Identifier: MIT
/**
 * @file rts_trace.h
 * @brief Trace file output utility.
 */
#ifndef RTS_TRACE_H
#define RTS_TRACE_H

#include <stddef.h>
#include <stdio.h>

/**
 * rts_trace_open - build trace file path and open it for writing
 * @outdir:     output directory (e.g., "output")
 * @task_path:  input task file path (e.g., "input/test1.txt")
 * @sched_name: scheduler name (e.g., "EDF")
 * @outpath:    buffer for returning full path
 * @outpath_sz: outpath buffer size
 *
 * Returns FILE pointer on success, or NULL on failure.
 */
FILE *rts_trace_open(const char *outdir,
                     const char *task_path,
                     const char *sched_name,
                     char *outpath, size_t outpath_sz);

#endif /* RTS_TRACE_H */
