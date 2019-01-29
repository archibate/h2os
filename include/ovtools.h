#pragma once

#define ovadd(x, y)      ((x) + (y) < (x))
#define ov_or_add(x, y)   ovadd(x, y) || ((x) + (y))
#define ov_and_add(x, y)  ovadd(x, y) && ((x) + (y))
#define nov_or_add(x, y)  !ovadd(x, y) || ((x) + (y))
#define nov_and_add(x, y) !ovadd(x, y) && ((x) + (y))
