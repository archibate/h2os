#include "sb.h"
#include "vn.h"
#include <h4/servers.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <bug.h>

static sb_t *sb;
vn_t *fat_root;

void fat_init(void)
{
	int hd = ipc_open(SVID_IDEDRV);
	BUG_ON(hd < 0);

	sb = load_sb(hd);
	BUG_ON(sb == NULL);

	fat_root = sb_openroot(sb);
}
