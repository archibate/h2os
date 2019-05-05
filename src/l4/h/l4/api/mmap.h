#pragma once

int sys_mmap(l4id_t mmc, l4fd_t fd, void *p, size_t size);
int sys_msync(l4id_t mmc, void *p, size_t size);
int sys_test_fault(l4id_t mmc, void *p, unsigned int errcd);
int sys_munmap(l4id_t mmc, void *p, size_t size);
