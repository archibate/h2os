#pragma once

struct endpoint;
sl4fd_t gf_open(struct endpoint *ep);
int gf_close(l4fd_t fd);
sl4fd_t gf_dup(l4fd_t fd);
int gf_dup2(l4fd_t fd, l4fd_t dirfd);
