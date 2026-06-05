
#ifndef UTILIX_TYPES_HPP
#define UTILIX_TYPES_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace utilix::types {

using u8 = std::uint8_t;
using i8 = std::int8_t;

using u16 = std::uint16_t;
using i16 = std::int16_t;

using u32 = std::uint32_t;
using i32 = std::int32_t;

using u64 = std::uint64_t;
using i64 = std::int64_t;

using ssize = std::size_t;

#ifndef NO_FLOATS
static_assert(sizeof(float) == 4, "Expected float to be 32 bits wide!");
static_assert(sizeof(double) == 8, "Expected double to be 64 bits wide!");

using f32 = float;
using f64 = double;
#endif

// Non owning mutable pointer.
template <typename T> using observer_ptr = T*;

template <typename T>
concept NumericInt = std::is_integral_v<T> && !std::is_same_v<T, bool>;

template <typename T>
concept UnsignedInt = NumericInt<T> && std::is_unsigned_v<T>;

} // namespace utilix::types

#endif // UTILIX_TYPES_HPP
