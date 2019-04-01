#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdarg.h>
#include <compiler.h>

typedef struct iobuf
{
	int fd;
	int err;
	int eof;
} FILE;

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern FILE *__stdfiles[3];

#define stdin  (__stdfiles[0])
#define stdout (__stdfiles[1])
#define stderr (__stdfiles[2])

int vfscanf(FILE *fp, const char *fmt, va_list ap);
int fscanf(FILE *fp, const char *fmt, ...) _FORMAT(scanf, 2, 3);
int vsscanf(const char *buf, const char *fmt, va_list ap);
int sscanf(const char *buf, const char *fmt, ...) _FORMAT(scanf, 2, 3);
int vscanf(const char *fmt, va_list ap);
int scanf(const char *fmt, ...) _FORMAT(scanf, 1, 2);
int vfprintf(FILE *fp, const char *fmt, va_list ap);
int fprintf(FILE *fp, const char *fmt, ...) _FORMAT(printf, 2, 3);
int vsprintf(char *buf, const char *fmt, va_list ap);
int sprintf(char *buf, const char *fmt, ...) _FORMAT(printf, 2, 3);
int vprintf(const char *fmt, va_list ap);
int printf(const char *fmt, ...) _FORMAT(printf, 1, 2);
int getchar(void);
int putchar(int ch);
char *gets(char *s);
int puts(const char *s);
FILE *fdopen(int fd, const char *mode);
FILE *fopen(const char *name, const char *mode);
FILE *freopen(const char *name, const char *mode, FILE *fp);
int fgetc(FILE *fp);
int fgetwc(FILE *fp);
int ungetc(int c, FILE *fp);
int ungetwc(int c, FILE *fp);
int fputc(int ch, FILE *fp);
int fputwc(int ch, FILE *fp);
#define getc fgetc
#define putc fputc
#define getwc fgetwc
#define putwc fputwc
int fputs(const char *s, FILE *fp);
char *fgets(char *s, size_t size, FILE *fp);
off_t fseek(FILE *fp, off_t off, int whence);
ssize_t fread(void *buf, size_t size, size_t nmemb, FILE *fp);
ssize_t fwrite(const void *buf, size_t size, size_t nmemb, FILE *fp);
off_t ftell(FILE *fp);
ssize_t fgetsize(FILE *fp);
int rewind(FILE *fp);
int feof(FILE *fp);
int ferror(FILE *fp);
int fflush(FILE *fp);
void clearerr(FILE *fp);
void fdclose(FILE *fp);
int fclose(FILE *fp);
