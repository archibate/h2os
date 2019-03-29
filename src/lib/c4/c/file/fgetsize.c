#include <stdio.h>

ssize_t fgetsize(FILE *fp)
{
	off_t cur = ftell(fp);
	if (cur < 0)
		return cur;
	off_t end = fseek(fp, 0, SEEK_END);
	fseek(fp, cur, SEEK_SET);
	return end;
}
