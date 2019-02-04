#pragma once

#include <inttypes.h>
#include <ccutils.h>

void _NORETURN move_to_user(const ulong *uc);
void _NORETURN goto_user_entry(void *pc, void *sp);
