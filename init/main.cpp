#include <asm/system.h>
#include <mm/memory.h>

using namespace toyOS;

extern char *_edata;
extern char *_end;

void Kernel::kernel_init(void)
{
	Console::init();
	Console::clearScreen();
	Console::debug_printf("[INFO] end: 0X%p, _edata: 0X%p\n", &_end, &_edata);
	Memory::init();

	while (true)
	{
		hlt();
	}
}
