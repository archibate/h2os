#pragma once

sl4id_t sys_rt_open(unsigned int rtype, l4id_t id, unsigned int flags);
int sys_rt_close(l4fd_t fd);
