#pragma once

#include <l4/types.h>
#include <l4/kobject.h>

typedef struct capability cap_t;

#include <l4/i/endpoint.h>
#include <l4/i/buffer.h>
#include <l4/i/cspace.h>
#include <l4/i/slab.h>
#include <l4/i/page.h>

typedef struct CSegment {
	word_t base;
	struct kobject ko;
	word_t limit;
	word_t water;
} CSegment_t;

typedef struct CIOSeg {
	word_t padding;
	struct kobject ko;
	half_t base;
	half_t limit;
} CIOSeg_t;

/* totally 32-bytes: */
struct capability
{
	byte_t ctype;
	byte_t _padding;
	union {
		void *c_objptr;
		word_t c_objaddr;
		endpoint_t c_endpoint;
		CSlab_t c_slab;
		CBuffer_t c_buffer;
		CSpace_t c_cspace;
		CPage_t c_page;
		CPgdir_t c_pgdir;
		CPgtab_t c_pgtab;
		CSegment_t c_segment;
		CIOSeg_t c_ioseg;
		word_t c_raw[4];
	};
};

#include <stassert.h>

static void __stassertF1212312(void)
{
	//static_assert(4 * sizeof(word_t) == sizeof(cap_t));
}
