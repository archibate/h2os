#pragma once

#ifdef __GNUC__

#define va_list __builtin_va_list
#define va_start(ap, head) __builtin_va_start(ap, head)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

#else

#define __$imaxsizeof(type) (sizeof(head) > sizeof(int) ? sizeof(head) : sizeof(int))
typedef char *va_list;
#define va_start(ap, head) ((ap) = (va_list) &(head) + __$imaxsizeof(head))
#define va_arg(ap, type) (*(type *)(((ap) += __$imaxsizeof(type)) - __$imaxsizeof(type)))
#define va_end(ap) ((ap) = (va_list) 0)

#endif
