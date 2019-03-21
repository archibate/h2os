#pragma once

#define ABS(a) ((a) > 0 ? (a) : -(a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(x, a, b) MIN(MAX(x, a), b)

#define CLMAX(x, max) do { if ((x) > (max)) (x) = (max); } while (0)
#define CLMIN(x, min) do { if ((x) < (min)) (x) = (min); } while (0)
