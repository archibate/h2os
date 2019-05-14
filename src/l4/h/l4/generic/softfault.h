#pragma once

struct ktcb;
struct fd_entry;
int softfault_callback(l4id_t mmc, word_t vaddr, unsigned int errcd);
void softfault_onreply(struct ktcb *target);
int softfault_mmap(l4id_t mmc, struct fd_entry *fde, word_t vaddr, size_t size, unsigned int flags, unsigned int prot);
int softfault_msync(l4id_t mmc, word_t vaddr, size_t size);
int softfault_munmap(l4id_t mmc, word_t vaddr, size_t size);
struct ktcb *softfault_fork_and_tcb(l4id_t mmc);
