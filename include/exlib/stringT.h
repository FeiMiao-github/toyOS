#ifndef __STRING_H__
#define __STRING_H__

static inline void *__memset_generic(void *s, char c, unsigned count) __attribute__((always_inline));

static inline void *__memset_generic(void *s, char c, unsigned count)
{
	int d0, d1;
	asm volatile("rep\n\t"
				 "stosb"
				 : "=&c"(d0), "=&D"(d1)
				 : "a"(c), "1"(s), "0"(count)
				 : "memory");
	return s;
}

/* we might want to write optimized versions of these later */
#define __constant_count_memset(s, c, count) __memset_generic((s), (c), (count))

#define memset(s, c, count)                           \
	(__builtin_constant_p(count)                      \
		 ? __constant_count_memset((s), (c), (count)) \
		 : __memset_generic((s), (c), (count)))

static inline void *memcpy(void *dest, const void *src, int n)
{
	__asm__("cld\n\t"
			"rep\n\t"
			"movsb" ::"c"(n),
			"S"(src), "D"(dest));
	return dest;
}

static inline int strlen(const char *s)
{
	int __res;
	__asm__("cld\n\t"
			"repne\n\t"
			"scasb\n\t"
			"notl %0\n\t"
			"decl %0"
			: "=c"(__res)
			: "D"(s), "a"(0), "0"(0xffffffff));
	return __res;
}

static inline char *
__strcpy(char *dst, const char *src)
{
	int d0, d1, d2;
	__asm__(
		"1: lodsb;"
		"stosb;"
		"testb %%al, %%al;"
		"jne 1b;"
		: "=&S"(d0), "=&D"(d1), "=&a"(d2)
		: "0"(src), "1"(dst)
		: "memory");
	return dst;
}

#endif
