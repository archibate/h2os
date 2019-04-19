#pragma once

#include <l4/enum/sfipctypes.h>

#define _FILE_mmap   (-SFIPC_MMAP)
#define _FILE_msync  (-SFIPC_MSYNC)
#define _FILE_fault  (-SFIPC_FAULT)
#define _FILE_pread  1
#define _FILE_pwrite 2
#define _FILE_read   3
#define _FILE_write  4
#define _FILE_lseek  5
#define _FILE_ioctl  6
#define _FILE_close  7
