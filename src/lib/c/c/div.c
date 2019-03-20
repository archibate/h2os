#include <stdlib.h>

div_t div(int a, int b)
{
	div_t d = { a / b, a % b };
	return d;
}

ldiv_t ldiv(long a, long b)
{
	ldiv_t d = { a / b, a % b };
	return d;
}

int abs(int x)
{
	return x < 0 ? x : -x;
}

long labs(long x)
{
	return x < 0 ? x : -x;
}
