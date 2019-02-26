#pragma once

sl4id_t sys_rt_new(unsigned int rtype);
sl4id_t sys_rt_open(unsigned int rtype, l4id_t id, unsigned int flags);
int sys_rt_check(unsigned int rtype, l4id_t id);
int sys_rt_delete(unsigned int rtype, l4id_t id);
int sys_rt_revoke(unsigned int rtype, l4id_t id);
