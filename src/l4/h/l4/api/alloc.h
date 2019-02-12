#pragma once

sl4id_t sys_new(unsigned int rtype);
int sys_delete(unsigned int rtype, l4id_t id);
int sys_revoke(unsigned int rtype, l4id_t id);
