#pragma once

struct ktcb;
struct fd_entry;
void softfault_callback(word_t vaddr, unsigned int errcd);
void softfault_onreply(struct ktcb *target);
int softfault_mmap(struct fd_entry *fde, word_t vaddr, size_t size, unsigned int flags);
int softfault_msync(word_t vaddr, size_t size);
