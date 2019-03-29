#include <stddef.h>
#include <errno.h>

static const char *err_msgs[] = {
#include "errmsgs.gen.inl"
};

char *strerror(int err)
{
	if (err < 0)
		err = -err;
	if (err >= array_sizeof(err_msgs)) {
		/*static char buf[32];
		sprintf(buf, "Error %d", err);
		return (char*)buf;*/
		return NULL;
	}
	return (char*)err_msgs[err];
}
