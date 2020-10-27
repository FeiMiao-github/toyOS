#ifndef _LIB_STD_TYPE_TRAITS_HPP_
#define _LIB_STD_TYPE_TRAITS_HPP_

template <typename T, T v>
struct integral_constant {
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant<T, v>;
  constexpr operator T() { return v; }
};

#endif
