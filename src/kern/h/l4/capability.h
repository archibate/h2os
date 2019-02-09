#pragma once

#include <l4/types.h>

/* all in 8-bytes: */
typedef struct CSlab {
	void *objptr;
	byte_t retype;
	byte_t _padding;
	half_t water;
} CSlab_t;

typedef struct CBuffer {
	word_t *objptr;
	half_t limit;
	half_t water;
} CBuffer_t;

typedef struct CCSpace {
	struct capability *objptr;
	half_t limit;
	half_t water;
} CCSpace_t;

typedef struct CPage {
	word_t objaddr;
} CPage_t, CPgtab_t, CPgdir_t;

typedef struct CSegment {
	word_t base : 32;
	word_t limit : 16;
	word_t water : 12;
} CSegment_t;

typedef struct CIOSeg {
	word_t padding;
	half_t base;
	half_t limit;
} CIOSeg_t;

typedef struct CEndpoint {
	struct Endpoint *objptr;
	byte_t retype;
	byte_t _padding1;
	half_t _padding2;
} CEndpoint_t;

/* totally 16-bytes: */
typedef struct capability
{
	byte_t ctype;
	byte_t _padding;
	short refcnt;
	struct capability *parent;
	union {
		void *c_objptr;
		word_t c_objaddr;
		CEndpoint_t c_endpoint;
		CSlab_t c_slab;
		CBuffer_t c_buffer;
		CCSpace_t c_cspace;
		CPage_t c_page;
		CPgdir_t c_pgdir;
		CPgtab_t c_pgtab;
		CSegment_t c_segment;
		CIOSeg_t c_ioseg;
	};
}
cap_t;

#include <stassert.h>

static void __stassertF1212312(void)
{
	static_assert(4 * sizeof(word_t) == sizeof(cap_t));
}
