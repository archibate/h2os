#pragma once

#define cli()       asm volatile ("cpsid i")
#define sti()       asm volatile ("cpsie i")
#define hlt()       asm volatile ("wfi")
#define stihlt()    asm volatile ("cpsie i;wfi")
#define stihltcli() asm volatile ("cpsie i;wfi;cpsid i")
#define sticli()    asm volatile ("cpsie i;cpsid i")
#define clihlt()    do { asm volatile ("cpsid i;wfi"); UNREACHABLE(); } while (1)
