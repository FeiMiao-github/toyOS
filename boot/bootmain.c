
#include <asm/io.h>
#include <asm/system.h>

#define SECTOR_SIZE 512
#define LOAD_KERNEL_ADDRESS (void *)0x10000

static void waitdisk(void)
{
	while ((inb(0x1F7) & 0xC0) != 0x40) /* do nothing */
		;
}

static void readsect(void *dst, unsigned int secno)
{
	waitdisk();

	outb(0x1F2, 0x1);														/* the num of sector to be read */
	outb(0x1F3, secno & 0xFF);									/* set start sector Logic sector num 7 ~ 0*/
	outb(0x1F4, (secno >> 8) && 0xFF);					/* Logic sector num 15 ~ 8 */
	outb(0x1F5, (secno >> 16) && 0xFF);					/* Logic sector num 23 ~ 16 */
	outb(0x1F6, 0xE0 | ((secno >> 24) && 0xF)); /* (0 CHS/ 1: LBA) | 1 | (0: Master-DISK | 1: Slave-DISK) | logic sector num 27 ~ 24 */
	outb(0x1F7, 0x20);													/* cmd 0x20 - read sectors */
	waitdisk();

	insl(0x1F0, dst, SECTOR_SIZE / 4);
}

static void readseg(void *dst, unsigned count, unsigned offset)
{
	void *end_addr = dst + count;

	unsigned sectno = offset / SECTOR_SIZE + 1;
	for (; dst < end_addr; dst += SECTOR_SIZE, sectno++)
	{
		readsect(dst, sectno);
	}
}

void bootmain()
{
	readseg(LOAD_KERNEL_ADDRESS, 16 * SECTOR_SIZE, 0);
	((void (*)(void))(LOAD_KERNEL_ADDRESS))();

	return;
}
