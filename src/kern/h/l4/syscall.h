#pragma once

#include <l4/types.h>
#include <l4/invoke.h>
#include <ccutils.h>

int _FASTCALL systemCall(cptr_t cptr, Invo_t *invo);
