#pragma once

#include <inttypes.h>

/* N: keep sync with c/kern/x86/isr/intrents.asm */
struct intr_frame
{
  /* PUSHADs: */
  ulong ifr_di, ifr_si, ifr_bp, ifr_oldsp;
  ulong ifr_bx, ifr_dx, ifr_cx, ifr_ax;
  /* segment regs */
  ulong ifr_gs, ifr_fs, ifr_es, ifr_ds;
  /* pushed in ents, before introute */
  ulong ifr_intnr, ifr_errcd; /* Intrrrupt No. & Error Code */
  /* pushed by hardware when intrrrupt occurred */
  ulong ifr_pc, ifr_cs, ifr_eflags;
  /* only intrrruption from user level has: */
  ulong ifr_sp, ifr_ss;
};
