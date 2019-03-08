#pragma once

#include <compiler.h>
#include <stdarg.h>

//#include <concolor.h>
//#define __PK_ANSI_RESET ANSI_RESET
#define KL_PANIC    "<0>"//ANSI_COL_L(RED)"PANIC"__PK_ANSI_RESET": "
#define KL_ALERT    "<1>"//ANSI_COL_L(MAGENTA)"ALERT"__PK_ANSI_RESET": "
#define KL_CRIT     "<2>"//ANSI_COL_L(CYAN)"CRIT"__PK_ANSI_RESET": "
#define KL_ERROR    "<3>"//ANSI_COL_L(RED)"ERROR"__PK_ANSI_RESET": "
#define KL_WARNING  "<4>"//ANSI_COL_L(YELLOW)"WARNING"__PK_ANSI_RESET": "
#define KL_NOTICE   "<5>"//ANSI_COL(YELLOW)"NOTICE"__PK_ANSI_RESET": "
#define KL_INFO     "<6>"//ANSI_COL(GREEN)"INFO"__PK_ANSI_RESET": "
#define KL_DEBUG    "<7>"//ANSI_COL_L(WHITE)"DEBUG"__PK_ANSI_RESET": "

#define DEFAULT_MESSAGE_LOGLEVEL  6 // KL_INFO
#ifdef CONFIG_DEBUG_LOG
#define DEFAULT_CONSOLE_LOGLEVEL  7 // KL_DEBUG
#define dprintk(...) printk(KL_DEBUG __VA_ARGS__)
#else
#define DEFAULT_CONSOLE_LOGLEVEL  6 // KL_INFO
#define dprintk(...) /* Nothing */
#endif

int printk(const char *fmt, ...) _FORMAT(printf, 1, 2);
int vprintk(const char *fmt, va_list ap);
void syslog(const char *s);
