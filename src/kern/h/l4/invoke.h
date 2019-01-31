#pragma once

#include <l4/types.h>
#include <l4/capability.h>

typedef struct Invo
{
	word_t service;
	union {
		word_t offset;
		word_t capCount;
	};
	word_t length;
	union {
		void *dataRecv;
		void const *dataSend;
		byte_t *byteRecv;
		byte_t const *byteSend;
		word_t *wordRecv;
		word_t const *wordSend;
	};
	cap_t *capDest;
}
Invo_t;

int sysInvoke(cap_t *target, Invo_t *invo);
