#pragma once

#define RoundDown(n,x)   (((x) / (n)) * (n))
#define RoundUp(n,x)     RoundDown(n, (x) + (n) - 1)
