#ifndef LIBS_STDARGT_H__
#define LIBS_STDARGT_H__

/* compiler provides size of save area */
typedef __builtin_va_list va_list;

#define va_start(ap, last) (__builtin_va_start(ap, last))
#define va_arg(ap, type) (__builtin_va_arg(ap, type))
#define va_end(ap) /*nothing*/

#endif // LIBS_STDARGT_H__