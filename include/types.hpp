
#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstddef>
#include <cstdint>

namespace utilix {

typedef std::uint8_t u8;
typedef std::int8_t i8;

typedef std::uint16_t u16;
typedef std::int16_t i16;

typedef std::uint32_t u32;
typedef std::int32_t i32;

typedef std::uint64_t u64;
typedef std::int64_t i64;

typedef std::size_t size;

#ifndef NO_FLOATS
static_assert(sizeof(float) == 4, "Expected float to be 32 bits wide!");
static_assert(sizeof(double) == 8, "Expected double to be 64 bits wide!");

typedef float f32;
typedef double f64;
#endif

// Non owning mutable pointer.
template <typename T> using observer_ptr = T *;

} // namespace utilix

#endif // !TYPES_HPP
