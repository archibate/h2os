#pragma once

#include <inttypes.h>

wchar_t *wcscpy(wchar_t *dst, const wchar_t *src);
wchar_t *wcsncpy(wchar_t *dst, const wchar_t *src, unsigned long n);
wchar_t *wcscat(wchar_t *dst, const wchar_t *src);
wchar_t *wcsncat(wchar_t *dst, const wchar_t *src, unsigned long n);
unsigned long wcslen(const wchar_t *src);
unsigned long wcsnlen(const wchar_t *src, unsigned long n);
int wcscmp(const wchar_t *dst, const wchar_t *src);
int wcsicmp(const wchar_t *dst, const wchar_t *src);
int wcsncmp(const wchar_t *dst, const wchar_t *src, unsigned long n);
int wcsnicmp(const wchar_t *dst, const wchar_t *src, unsigned long n);
wchar_t *wcsndup(const wchar_t *s, unsigned long n);
wchar_t *wcsdup(const wchar_t *s);
wchar_t *wcschr(const wchar_t *src, int ch);
wchar_t *wcschrl(const wchar_t *src, int ch);
wchar_t *wcsnchr(const wchar_t *src, int ch, unsigned long n);
wchar_t *wcsnchrl(const wchar_t *src, int ch, unsigned long n);
wchar_t *wcschrin(const wchar_t *src, const wchar_t *chrs);
wchar_t *wcschrlin(const wchar_t *src, const wchar_t *chrs);
wchar_t *wcsskipin(const wchar_t *dst, const wchar_t *chrs);
int wcschopin(wchar_t *dst, const wchar_t *chrs);
wchar_t *wcstrimin(wchar_t *dst, const wchar_t *chrs);
wchar_t *wcserror(int err);
