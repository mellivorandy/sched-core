// SPDX-License-Identifier: MIT
/**
 * @file rts_trace.c
 * @brief Trace file output utility.
 */
#include "rts_trace.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define RTS_DIR_MODE 0755
#define RTS_PATH_MAX 512

#ifdef _WIN32
#include <direct.h>   	/* _mkdir */
#define RTS_MKDIR(p) _mkdir(p)
#define PATH_SEP '\\'
#else
#include <sys/stat.h> 	/* mkdir */
#define RTS_MKDIR(p) mkdir(p, RTS_DIR_MODE)
#define PATH_SEP '/'
#endif

static void mkdir_ignore_exists(const char *path) {
	if (!path || !*path)
		return;
	
	if (RTS_MKDIR(path) == -1) {
        if (errno != EEXIST) {
            fprintf(stderr, "[trace] cannot create directory: %s\n", path);
            perror("mkdir");
        }
    }
}

static void rts_path_stem(const char *path, char *out, size_t outsz) {
	const char *base;
    const char *s1, *s2, *slash;

    if (!out || outsz == 0)
        return;

    if (!path) {
        snprintf(out, outsz, "trace");
        return;
    }

    s1 = strrchr(path, '/');
    s2 = strrchr(path, '\\');

    slash = s1;
    if (s2 && (!slash || s2 > slash))
        slash = s2;

    base = slash ? slash + 1 : path;

    snprintf(out, outsz, "%s", base);

    char *dot = strrchr(out, '.');
    if (dot && dot != out) {
        *dot = '\0';
    }
}

FILE *rts_trace_open(const char *outdir,
                     const char *task_path,
                     const char *sched_name,
                     char *outpath, size_t outpath_sz) {
	char base[256];
	rts_path_stem(task_path, base, sizeof(base));

	char trace_dir[RTS_PATH_MAX];
	if (outdir && *outdir) {
		mkdir_ignore_exists(outdir);
		snprintf(trace_dir, sizeof(trace_dir), "%s%c%s", outdir, PATH_SEP, base);
		mkdir_ignore_exists(trace_dir);
	} else {
		snprintf(trace_dir, sizeof(trace_dir), "%s", base);
		mkdir_ignore_exists(trace_dir);
	}

	const char *sched_tag = (sched_name && *sched_name) ? sched_name : "trace";

	int full_len = snprintf(NULL, 0, "%s%c%s.txt", trace_dir, PATH_SEP, sched_tag) + 1;
	if (full_len <= 0)
		return NULL;

	char *full = (char *)malloc((size_t)full_len);
	if (!full)
		return NULL;

	(void)snprintf(full, (size_t)full_len, "%s%c%s.txt", trace_dir, PATH_SEP, sched_tag);

	if (outpath && outpath_sz > 0) {
		(void)snprintf(outpath, outpath_sz, "%s", full);
	}

	FILE *fp = fopen(full, "w");
	free(full);
	return fp;
}
