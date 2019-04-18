#pragma once

int sys_mmap(l4fd_t fd, void *p, size_t size, unsigned int flags);
int sys_msync(void *p, size_t size);
int sys_test_fault(void *p, unsigned int errcd);
