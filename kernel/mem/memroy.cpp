#include <mm/memory.h>

using namespace toyOS;

static int *E820MemInfo_MAP_NUM = (int *)0x8000;
static struct E820MemInfo
{
	int *addr[2];
	int *size[2];
	int type;
} const *st_E820_MEMINFO = (E820MemInfo *)0x8004;

void Memory::init(void)
{
	Console::debug_printf("[INFO] E820_MEMINFO total %lx\n", *E820MemInfo_MAP_NUM);
	for (int i = 0; i < 6; i++)
	{
		Console::debug_printf("[INFO] E820_MEMINFO[%d] addrh: 0X%x, addrl: 0X%x, type: %d\n", 0, st_E820_MEMINFO[i].addr[1], st_E820_MEMINFO[i].addr[0], st_E820_MEMINFO[i].type);
	}
}
