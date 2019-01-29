#pragma once

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a, b) ((a) > 0 ? (a) : -(a))
#define CLAMP(x, a, b) MIN(MAX(x, a), b)
