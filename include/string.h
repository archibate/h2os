#pragma once

char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, unsigned long n);
char *strcat(char *dst, const char *src);
char *strncat(char *dst, const char *src, unsigned long n);
unsigned long strlen(const char *src);
unsigned long strnlen(const char *src, unsigned long n);
int strcmp(const char *dst, const char *src);
int stricmp(const char *dst, const char *src);
int strncmp(const char *dst, const char *src, unsigned long n);
int strnicmp(const char *dst, const char *src, unsigned long n);
char *strndup(const char *s, unsigned long n);
char *strdup(const char *s);
char *strchr(const char *src, int ch);
char *strchrl(const char *src, int ch);
char *strnchr(const char *src, int ch, unsigned long n);
char *strnchrl(const char *src, int ch, unsigned long n);
char *strchrin(const char *src, const char *chrs);
char *strchrlin(const char *src, const char *chrs);
char *strskipin(const char *dst, const char *chrs);
int strchopin(char *dst, const char *chrs);
char *strtrimin(char *dst, const char *chrs);
char *strerror(int err);
