#include <catch2/catch_test_macros.hpp>
#include <utilix/types.hpp>

#include <cstdint>
#include <type_traits>

using namespace utilix::types;

TEST_CASE("integer aliases have the expected widths", "[types]") {
  STATIC_REQUIRE(sizeof(u8) == 1);
  STATIC_REQUIRE(sizeof(i8) == 1);
  STATIC_REQUIRE(sizeof(u16) == 2);
  STATIC_REQUIRE(sizeof(i16) == 2);
  STATIC_REQUIRE(sizeof(u32) == 4);
  STATIC_REQUIRE(sizeof(i32) == 4);
  STATIC_REQUIRE(sizeof(u64) == 8);
  STATIC_REQUIRE(sizeof(i64) == 8);

  STATIC_REQUIRE(std::is_same_v<u8, std::uint8_t>);
  STATIC_REQUIRE(std::is_same_v<i64, std::int64_t>);
  STATIC_REQUIRE(std::is_same_v<ssize, std::size_t>);
}

#ifndef NO_FLOATS
TEST_CASE("float aliases have the expected widths", "[types]") {
  STATIC_REQUIRE(sizeof(f32) == 4);
  STATIC_REQUIRE(sizeof(f64) == 8);
}
#endif

TEST_CASE("observer_ptr is a raw pointer alias", "[types]") {
  STATIC_REQUIRE(std::is_same_v<observer_ptr<int>, int*>);

  // volatile so static analysers can't fold the read.
  volatile int value = 42;
  observer_ptr<volatile int> p = &value;
  REQUIRE(*p == 42);
}

TEST_CASE("NumericInt accepts integers but not bool", "[types]") {
  STATIC_REQUIRE(NumericInt<int>);
  STATIC_REQUIRE(NumericInt<u64>);
  STATIC_REQUIRE(NumericInt<i32>);
  STATIC_REQUIRE_FALSE(NumericInt<bool>);
  STATIC_REQUIRE_FALSE(NumericInt<float>);
  STATIC_REQUIRE_FALSE(NumericInt<double>);
}

TEST_CASE("UnsignedInt accepts only unsigned integers", "[types]") {
  STATIC_REQUIRE(UnsignedInt<u8>);
  STATIC_REQUIRE(UnsignedInt<u64>);
  STATIC_REQUIRE(UnsignedInt<unsigned>);
  STATIC_REQUIRE_FALSE(UnsignedInt<i32>);
  STATIC_REQUIRE_FALSE(UnsignedInt<int>);
  STATIC_REQUIRE_FALSE(UnsignedInt<bool>);
}
