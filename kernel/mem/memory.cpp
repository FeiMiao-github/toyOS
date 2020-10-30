#include <mm/memory.h>

#if DEBUG
#include <debug/debug.h>
#endif

using namespace toyOS;
static int *E820MemInfo_MAP_NUM = (int *)0x8000;
static struct E820MemInfo
{
	int *addr[2];
	int *size[2];
	int type;
} const *st_E820_MEMINFO = (E820MemInfo *)0x8004;

unsigned char Memory::mem_map[PAGE_SIZE] = {
	0,
};

void Memory::init(void)
{
	Console::debug_printf("[INFO] E820_MEMINFO total %lx\n", *E820MemInfo_MAP_NUM);
	for (int i = 0; i < 6; i++)
	{
		Console::debug_printf("[INFO] E820_MEMINFO[%d] addr: 0X%x%x, size: 0X%x%x, type: %d\n",
							  0, st_E820_MEMINFO[i].addr[1], st_E820_MEMINFO[i].addr[0],
							  st_E820_MEMINFO[i].size[1], st_E820_MEMINFO[i].size[0],
							  st_E820_MEMINFO[i].type);
	}
#ifdef DEBUG
	Debug::printStack();
#endif
	Console::debug_printf("0X%x", MB_2_BYTE<MEMORY_SIZE_MB>::value >> 12);
	for (unsigned i = 0; i < PAGE_SIZE; i++)
	{
		mem_map[i] = static_cast<unsigned char>(Memory::MEM_FLAG::USED);
	}
}
