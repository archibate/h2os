#include <l4/generic/msginfo.h>

#if 0
void msginfo_copy(struct msginfo *dst, const struct msginfo *src)
{
	memcpy(dst, src, sizeof(*dst));
	dst->rplfd = gf_dup(src->rplfd);
}
#endif
