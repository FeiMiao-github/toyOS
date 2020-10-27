#ifndef MEMORY_H__
#define MEMORY_H__

#include "../toyOS.h"

namespace toyOS
{
	class Memory
	{
		class Memm
		{
			void *memmap[1024];

		public:
			static void init(void);
		};

	public:
		static void init(void);
	};
} // namespace toyOS

#endif // MEMORY_H__
