#include <l4/sys/sysnr.h>
#include <l4/api/asyncep.h>
#include <l4/api/endpoint.h>
#include <l4/api/hello.h>
#include <l4/api/rtalloc.h>
#include <l4/api/sched.h>
#include <l4/api/softirq.h>
#include <l4/api/thread.h>

void *_systab[] = {
