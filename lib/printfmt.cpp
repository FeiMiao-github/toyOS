#include <exlib/printfmtT.h>
#include <exlib/stringT.h>
#include <std/templateType.hpp>

#define do_div(n, base) ({                               \
	unsigned long __upper, __low, __high, __mod, __base; \
	__base = (base);                                     \
	asm(""                                               \
		: "=a"(__low), "=d"(__high)                      \
		: "A"(n));                                       \
	__upper = __high;                                    \
	if (__high != 0)                                     \
	{                                                    \
		__upper = __high % __base;                       \
		__high = __high / __base;                        \
	}                                                    \
	asm("divl %2"                                        \
		: "=a"(__low), "=d"(__mod)                       \
		: "rm"(__base), "0"(__low), "1"(__upper));       \
	asm(""                                               \
		: "=A"(n)                                        \
		: "a"(__low), "d"(__high));                      \
	__mod;                                               \
})

static unsigned short putcNumber(long long src, char *dst, unsigned short base);
static unsigned short putstr(va_list &ap, char *dst);
static unsigned short putcPointer(va_list &ap, char *dst);

struct Signed
{
	using typeLongLong = long long;
	using typeLong = long;
	using type = int;
	static const typeLongLong value = 1;
};

struct Unsigned
{
	using typeLongLong = unsigned long long;
	using typeLong = unsigned long;
	using type = unsigned;
	static const typeLongLong value = 1;
};

template <typename T>
static const typename T::typeLongLong numVal(va_list &ap, unsigned short lflag)
{
	typename T::typeLongLong num;
	if (lflag > 1)
	{
		num = va_arg(ap, typename T::typeLongLong);
	}
	else if (lflag > 0)
	{
		num = va_arg(ap, typename T::typeLong);
	}
	else
	{
		num = va_arg(ap, typename T::type);
	}

	return num;
}

void printFmt(char *dst, unsigned dstlen, const char *fmt, va_list &ap)
{
	char ch;
	unsigned short lflag = 0;
	unsigned short i = 0;
	unsigned long long num = 0;

	memset(dst, 0, dstlen);

	while ((ch = *(unsigned char *)fmt++) != '\0')
	{
		if (ch != '%')
		{
			dst[i++] = ch;
			continue;
		}

	RE_SWITCH:
		ch = *(unsigned char *)fmt++;
		switch (ch)
		{
		case 'l':
			lflag++;
			goto RE_SWITCH;
		case 'u':
			num = numVal<Unsigned>(ap, lflag);
			i += putcNumber(num, &dst[i], 10);
			break;
		case 'd':
			num = numVal<Signed>(ap, lflag);
			if ((long long)num < 0)
			{
				dst[dstlen++] = '-';
				num = -(long long)num;
			}
			i += putcNumber(num, &dst[i], 10);
			break;
		case 'b':
			num = numVal<Unsigned>(ap, lflag);
			i += putcNumber(num, &dst[i], 2);
			break;
		case 'o':
			num = numVal<Unsigned>(ap, lflag);
			i += putcNumber(num, &dst[i], 8);
			break;
		case 'x':
			num = numVal<Unsigned>(ap, lflag);
			i += putcNumber(num, &dst[i], 16);
			break;
		case 's':
			i += putstr(ap, &dst[i]);
			break;
		case 'p':
			i += putcPointer(ap, &dst[i]);
			break;
		default:
			break;
		}
	}
	dst[i] = '\0';
}

/**
 * @param {
 *   src: source number
 *   dst: destination string
 * 	 base: bin 2
 * 		   oct 8
 * 		   dec 10
 * 		   hex 16
 * }
 * 
 * @result destination string length
*/
static unsigned short putcNumber(long long src, char *dst, unsigned short base)
{
	const short MAX_NUMBER_CHAR = 128;
	char numberStr[MAX_NUMBER_CHAR] = {0};
	int numberPos = MAX_NUMBER_CHAR;
	const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char ch;
	unsigned short dstlen = 0;

	if (src == 0)
	{
		dst[dstlen++] = '0';
	}
	else
	{
		while (src && numberPos > 0)
		{
			numberStr[numberPos - 1] = digits[do_div(src, base)];
			numberPos--;
		}
	}

	for (int ibit = 0; ibit < MAX_NUMBER_CHAR; ibit++)
	{
		ch = numberStr[ibit];
		if (ch != 0)
		{
			dst[dstlen++] = ch;
		}
	}

	return dstlen;
}

/**
 * @param {
 *   ap: arguments list
 *   dst: destination string
 * }
 * 
 * @result destination string length
*/
static unsigned short putstr(va_list &ap, char *dst)
{
	unsigned short i = 0;
	const char nulltext[] = "<NULL>";
	char *str;

	str = va_arg(ap, char *);

	if (!str)
	{
		i = strlen(nulltext);
		memcpy(dst, nulltext, i);
	}
	else
	{
		while (*str != '\0')
		{
			dst[i++] = *str;
			str += 1;
		}
	}

	return i;
}

/**
 * @param {
 *   ap: arguments list
 *   dst: destination string
 * }
 * 
 * @result destination string length
*/
static unsigned short putcPointer(va_list &ap, char *dst)
{
	unsigned short i = 0;
	// const char nulltext[] = "<NULL>";
	unsigned long addr = (unsigned long)(va_arg(ap, void *));
	// if (!addr)
	// {
	// 	i = strlen(nulltext);
	// 	memcpy(dst, nulltext, i);
	// }
	// else
	{
		i = putcNumber((unsigned long)(addr), dst, 16);
	}
	return i;
}
