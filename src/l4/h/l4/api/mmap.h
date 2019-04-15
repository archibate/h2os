#pragma once

int sys_mmap(l4fd_t fd, void *p, size_t size, unsigned int mattr);
