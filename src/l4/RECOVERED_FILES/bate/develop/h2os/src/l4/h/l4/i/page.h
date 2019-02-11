#pragma once

typedef struct CPage {
	word_t objaddr;
	struct kobject ko;
} CPage_t, CPgtab_t, CPgdir_t;
