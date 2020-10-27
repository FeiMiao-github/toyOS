#ifndef __IO_H__
#define __IO_H__

static inline void outb(unsigned short port, unsigned char value) __attribute__((always_inline));
static inline unsigned char inb(unsigned short port) __attribute__((always_inline));
static inline void insl(unsigned short port, void *addr, unsigned long cnt) __attribute__((always_inline));

static inline void outb(unsigned short port, unsigned char value) {
	__asm__ __volatile__("outb %0, %1"::"a"(value), "d"(port));
}

static inline unsigned char inb(unsigned short port) {
	register unsigned char value __asm__ ("al");
	__asm__ __volatile__("inb %1, %0": "=a"(value) : "d"(port));
	return value;
}

static inline void insl(unsigned short port, void *addr, unsigned long cnt) {
	__asm__ __volatile__ (
			"cld;"
			"repne; insl;"
			: "=D" (addr), "=c" (cnt)
			: "d" (port), "0" (addr), "1" (cnt)
			: "memory", "cc");
}

#endif /* __IO_H__ */

