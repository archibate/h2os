#pragma once

#include <inttypes.h>

typedef uint32_t clock_t;
typedef uint32_t time_t;

struct tm {
	int tm_sec; /* 0 to 59 */
	int tm_min; /* 0 to 59 */
	int tm_hour; /* 0 to 23 */
	int tm_mday; /* 1 to 31 */
	int tm_mon; /* 0 to 11 */
	int tm_year; /* Since 1990 */
	int tm_wday; /* 0 to 6 */
	int tm_yday; /* 0 to 365 */
};

clock_t clock(void);
