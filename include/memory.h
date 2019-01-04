#pragma once


void *memcpy(void *dst, const void *src, unsigned long n);
void *memrcpy(void *dst, const void *src, unsigned long n);
int memcmp(const void *dst, const void *src, unsigned long n);
void *memsetw(void *dst, int val, unsigned long n);
void *memset(void *dst, int val, unsigned long n);
void *memchr(const void *dst, int chr, unsigned long n);
void *memchrl(const void *dst, int chr, unsigned long n);

#define memmove(dst, src, n) ((dst > src) ? memrcpy(dst, src, n) : memcpy(dst, src, n))
#define memzero(dst, n) memset(dst, 0, n)
#define bcopy(src, dst, n) memcpy(dst, src, n)
#define bzero(dst, n) memzero(dst, n)
