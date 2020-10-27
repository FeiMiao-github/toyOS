#ifndef __SYSTEM_H__
#define __SYSTEM_H__

typedef enum {
	CHAR_80x25_16_B800 = 0x02,
	SHAPE_640x480_16_A000 = 0x12,
	SHAPE_320x200_256_A000 = 0x13
} VGADisplayMode;

static inline void hlt(void) __attribute__ ((always_inline));
static inline void setVGADisplayMode(VGADisplayMode mode) __attribute__ ((always_inline));

static inline void hlt(void) {
	__asm__ ("hlt");
}

static inline void setVGADisplayMode(VGADisplayMode mode) {
	__asm__ __volatile__("int $0x10"::"a"(mode));
}

#endif /* __SYSTEM_H__ */