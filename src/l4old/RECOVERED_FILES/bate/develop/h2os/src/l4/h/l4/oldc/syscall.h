#pragma once

#include <l4/types.h>
#include <ccutils.h>

int _FASTCALL systemCall(cptr_t cptr, word_t *shortMsg);
