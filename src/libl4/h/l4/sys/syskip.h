#pragma once

#ifdef _MINGW
#define _SYSKIP_SYM_syscall_in "_libl4_do_syscall"
#else
#define _SYSKIP_SYM_syscall_in "libl4_do_syscall"
#endif
