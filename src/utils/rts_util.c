// SPDX-License-Identifier: MIT
/**
 * @file rts_util.c
 * @brief Utility function implementations.
 */
#include "rts_util.h"

int rts_util_gcd(int a, int b) {
	while (b != 0) {
		int t = b;
		b = a % b;
		a = t;
	}
	return a;
}

int rts_util_lcm(int a, int b) {
	return (a * b) / rts_util_gcd(a, b);
}

int rts_util_hyperperiod(const int *periods, int n) {
	int result = periods[0];

	for (int i = 1; i < n; i++) {
		result = rts_util_lcm(result, periods[i]);
	}

	return result;
}
