#ifndef __TOYOS_H__
#define __TOYOS_H__

namespace toyOS
{
	class Kernel
	{
	public:
		static void kernel_init(void);
	};

	class Console
	{
		struct Position
		{
			unsigned short row;
			unsigned short col;
		};

		static Position curPos_;

		void putchar(char ch);

	public:
		static void init(void);
		static void debug_printf(const char *str, ...);
		static void clearScreen(void);
		void operator<<(const char *str);
	};
	class Memory;
} // namespace toyOS

#endif /* __TOYOS_H__ */
