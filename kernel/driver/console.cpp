#include <toyOS.h>
#include <exlib/printfmtT.h>

#define _CHAR_VIDEOBUFFER_START 0xb8000
#define _CHAR_BUFFER_LEN 1000 * 4
#define _SHAPE_VIDEOBUFFER_START 0xa0000
#define _SHAPE_VIDEOBUFFER_LEN (0xffff - 0X5FF) /* 320*200*8 = 64000 */

#define MONO_BASE 0x3B4
#define MONO_BUF 0xB0000
#define CGA_BASE 0x3D4
#define CGA_BUF 0xB8000
#define CRT_ROWS 25
#define CRT_COLS 80
#define CRT_SIZE (CRT_ROWS * CRT_COLS)

#define LPTPORT 0x378

using namespace toyOS;

namespace toyOS
{
	class CGA
	{
	public:
		static void init();
	};
} // namespace toyOS

Console::Position Console::curPos_ = {0, 0};

void Console::init(void)
{
}

void Console::putchar(char ch)
{
}

void Console::debug_printf(const char *fmt, ...)
{
	constexpr const unsigned short CHAR_BUF_SIZE = 1024;
	char buf[CHAR_BUF_SIZE];

	va_list ap;
	va_start(ap, fmt);
	printFmt(buf, CHAR_BUF_SIZE, fmt, ap);
	va_end(ap);

	char *pBuf = reinterpret_cast<char *>(_CHAR_VIDEOBUFFER_START);
	unsigned pos = Console::curPos_.row * CRT_COLS + Console::curPos_.col;
	unsigned i = 0;
	unsigned hasShowNum = 0;
	while (buf[i] != '\0')
	{
		if (buf[i] == '\n')
		{
			const int space = CRT_COLS - i % CRT_COLS;
			for (int j = 0; j < space; j++)
			{
				pBuf[2 * i + 2 * pos] = buf[i];
				pBuf[2 * i + 1 + 2 * pos] = 0x0;
				i++;
				hasShowNum++;
			}
		}
		else
		{
			pBuf[2 * i + 2 * pos] = buf[i];
			pBuf[2 * i + 1 + 2 * pos] = 0x0F;
			i++;
			hasShowNum++;
		}
	}

	Console::curPos_.row = (pos + hasShowNum) / CRT_COLS;
	Console::curPos_.col = (pos + hasShowNum) % CRT_COLS;
	if (Console::curPos_.row > CRT_ROWS ||
		(Console::curPos_.row == CRT_ROWS && Console::curPos_.col > 0))
	{
		clearScreen();
	}
}

void Console::clearScreen(void)
{
	char *pBuf = reinterpret_cast<char *>(_CHAR_VIDEOBUFFER_START);
	for (unsigned i = 0; i < CRT_SIZE; i++)
	{
		pBuf[i] = 0x0;
	}

	Console::curPos_.row = 0;
	Console::curPos_.col = 0;
}

void Console::operator<<(const char *str)
{
}

void CGA::init()
{
}
