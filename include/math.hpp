#ifndef UTILIX_MATH_HPP
#define UTILIX_MATH_HPP

#include <type_traits>

template <typename T>
concept NumericInt = std::is_integral_v<T> && !std::is_same_v<T, bool>;

template <typename T>
concept UnsignedInt = NumericInt<T> && std::is_unsigned_v<T>;

template <NumericInt A, UnsignedInt B>
constexpr B euc_mod(A a, B b)
  requires(sizeof(B) >= sizeof(A))
{
  B r = static_cast<B>(a) % b;
  return (a < A{0} and r != 0) ? b - r : r;
}

#endif // UTILIX_MATH_HPP
