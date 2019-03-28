#include "sb.h"
#include "vn.h"
#include "de.h"

vn_t *sb_openroot(sb_t *sb)
{
	de_t e;
	e.attr = T_DIR;
	e.size = sb->root_ents * DESIZE;
	esetclus(&e, 0);
	return __vopen(sb, &e);
}
