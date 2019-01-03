#pragma once

typedef unsigned short wchar_t;

wchar_t *wcscpy(wchar_t *dst, const wchar_t *src);
wchar_t *wcsncpy(wchar_t *dst, const wchar_t *src, unsigned long n);
wchar_t *wcscat(wchar_t *dst, const wchar_t *src);
wchar_t *wcsncat(wchar_t *dst, const wchar_t *src, unsigned long n);
unsigned long wcslen(const wchar_t *src);
unsigned long wcsnlen(const wchar_t *src, unsigned long n);
int wcscmp(const wchar_t *dst, const wchar_t *src);
int wcsncmp(const wchar_t *dst, const wchar_t *src, unsigned long n);
wchar_t *wcsndup(const wchar_t *s, unsigned long n);
wchar_t *wcsdup(const wchar_t *s);
wchar_t *wcschr(const wchar_t *src, wchar_t ch);
wchar_t *wcschrl(const wchar_t *src, wchar_t ch);
wchar_t *wcsnchr(const wchar_t *src, wchar_t ch, unsigned long n);
wchar_t *wcsnchrl(const wchar_t *src, wchar_t ch, unsigned long n);
wchar_t *wcschrin(const wchar_t *src, const wchar_t *chrs);
wchar_t *wcschrlin(const wchar_t *src, const wchar_t *chrs);
wchar_t *wcsskipin(const wchar_t *dst, const wchar_t *chrs);
int wcschop(wchar_t *dst, const wchar_t *chrs);
wchar_t *wcstrim(wchar_t *dst, const wchar_t *chrs);
const wchar_t *wcserror(int err);
