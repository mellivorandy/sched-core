// SPDX-License-Identifier: MIT
/**
 * @file rts_util.h
 * @brief Utility functions.
 */
#ifndef RTS_UTIL_H
#define RTS_UTIL_H

int rts_util_gcd(int a, int b);
int rts_util_lcm(int a, int b);
int rts_util_hyperperiod(const int *arr, int n);

static inline int rts_min_int(int a, int b)
{
	return (a < b) ? a : b;
}

static inline int rts_max_int(int a, int b)
{
	return (a > b) ? a : b;
}

#endif /* RTS_UTIL_H */
