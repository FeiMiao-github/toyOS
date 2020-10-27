#ifndef STD_TEMPLATE_TYPE_H__
#define STD_TEMPLATE_TYPE_H__

struct Boolean_tag
{
	static const bool is_boolean = true;
	static const bool is_number = false;
};

struct Number_tag
{
	static const bool is_boolean = false;
	static const bool is_number = true;
};

template <bool flag>
struct Boolean
{
	using value_type = bool;
	using tag = Boolean_tag;
	static const bool value = flag;
};

template <long long val>
struct LongLong
{
	using value_type = long long;
	using tag = Number_tag;
	static const bool signed_tag = true;
	static const long long value = val;
};

template <unsigned long long val>
struct UnsignedLongLong
{
	using value_type = unsigned long long;
	using tag = Number_tag;
	static const bool signed_tag = false;
	static const long long value = val;
};

template <typename T>
using is_number = Boolean<T::tag::is_number>;

template <typename T>
using is_boolean = Boolean<T::tag::is_boolean>;

template <typename T>
using is_signed = Boolean<T::signed_tag>;

#endif
