#include "sb.h"
#include "vn.h"
#include <h4/servers.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/sys/sched.h>
#include <bug.h>

static sb_t *sb;
vn_t *fat_root;

void fat_init(void)
{
	int hd;
	while ((hd = ipc_open(SVID_IDEDRV)) < 0)
		pause();

	sb = load_sb(hd);
	BUG_ON(sb == NULL);

	fat_root = sb_openroot(sb);
}
