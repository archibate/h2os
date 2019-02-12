#pragma once

void rtype_init(void *p, unsigned int rtype);
void rtype_revoke(void *p, unsigned int rtype);
void rtype_delete(void *p, unsigned int rtype);
size_t rtype_get_sizeof(unsigned int rtype);
