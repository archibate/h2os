#include <stdio.h>
#include <compiler.h>

FILE *__stdfiles[3];

static void _CTOR stdio_ctor(void)
{
	stdin = fdopen(0, "r");
	stdout = fdopen(1, "w");
	stderr = fdopen(2, "w");
}
