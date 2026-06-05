#ifndef UTILIX_MATH_HPP
#define UTILIX_MATH_HPP

#include "./types.hpp"

namespace utilix::math {

template <types::NumericInt A, types::UnsignedInt B>
constexpr B euc_mod(A a, B b)
  requires(sizeof(B) >= sizeof(A))
{
  B r = static_cast<B>(a) % b;
  return (a < A{0} and r != 0) ? b - r : r;
}

} // namespace utilix::math

#endif // UTILIX_MATH_HPP
