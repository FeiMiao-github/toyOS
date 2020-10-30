#include <debug/debug.h>
#include <toyOS.h>

using console = toyOS::Console;

static unsigned read_ebp(void)
{
	unsigned ebp;
	asm volatile("movl %%ebp, %0\n\t"
				 : "=a"(ebp));
	return ebp;
}

static unsigned read_eip(void) __attribute__((noinline));
unsigned read_eip(void)
{
	unsigned eip;
	asm volatile("movl (%%esp), %0\n\t"
				 : "=r"(eip));
	return eip;
}

void Debug::printStack()
{
	unsigned ebp = read_ebp();
	unsigned eip = read_eip();
	console::debug_printf("ebp: 0x%x, eip: 0x%x\n", ebp, eip);
}
