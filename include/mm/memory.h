#ifndef MEMORY_H__
#define MEMORY_H__

#include "../toyOS.h"

/* -----------------------------------| address     |
 * type: 1
 * -----------------------------------| 0x100000    |
 * type: 2
 * -----------------------------------| 0x9FC00     |
 * type: 1  
 * -----------------------------------| 0           | 
 */
template <int v>
struct MB_2_BYTE
{
	/* data */
	static constexpr unsigned value = v << 20;
};

namespace toyOS
{
	class Memory
	{
		static constexpr unsigned MEMORY_SIZE_MB = 128;
		static constexpr unsigned PAGE_SIZE = MB_2_BYTE<MEMORY_SIZE_MB>::value >> 12;
		static unsigned char mem_map[PAGE_SIZE];

		enum class MEM_FLAG : unsigned char
		{
			USED = 100,
		};

	public:
		void
		init(void);
	};
} // namespace toyOS

#endif // MEMORY_H__
