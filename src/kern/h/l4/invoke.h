#pragma once

#include <l4/types.h>
#include <l4/capability.h>

typedef struct Invo
{
	word_t service;
	word_t offset;
	word_t length;
	union {
		byte_t *dataRecv;
		byte_t const *dataSend;
	};
}
Invo_t;

int sysInvoke(cap_t *target, Invo_t *invo);
