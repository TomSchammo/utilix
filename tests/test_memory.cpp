#include <catch2/catch_test_macros.hpp>
#include <utilix/memory.hpp>

using namespace utilix::memory;

TEST_CASE("decimal byte conversions are powers of 1000", "[memory]") {
  STATIC_REQUIRE(kilo_bytes(1) == 1000ULL);
  STATIC_REQUIRE(mega_bytes(1) == 1000ULL * 1000ULL);
  STATIC_REQUIRE(giga_bytes(1) == 1000ULL * 1000ULL * 1000ULL);

  STATIC_REQUIRE(kilo_bytes(0) == 0ULL);
  STATIC_REQUIRE(mega_bytes(0) == 0ULL);
  STATIC_REQUIRE(giga_bytes(0) == 0ULL);

  STATIC_REQUIRE(kilo_bytes(4) == 4ULL * 1000ULL);
  STATIC_REQUIRE(mega_bytes(8) == 8ULL * 1000ULL * 1000ULL);
  STATIC_REQUIRE(giga_bytes(2) == 2ULL * 1000ULL * 1000ULL * 1000ULL);
}

TEST_CASE("binary byte conversions are powers of 1024", "[memory]") {
  STATIC_REQUIRE(kibi_bytes(1) == 1024ULL);
  STATIC_REQUIRE(mibi_bytes(1) == 1024ULL * 1024ULL);
  STATIC_REQUIRE(gibi_bytes(1) == 1024ULL * 1024ULL * 1024ULL);

  STATIC_REQUIRE(kibi_bytes(0) == 0ULL);
  STATIC_REQUIRE(mibi_bytes(0) == 0ULL);
  STATIC_REQUIRE(gibi_bytes(0) == 0ULL);

  STATIC_REQUIRE(kibi_bytes(4) == 4ULL * 1024ULL);
  STATIC_REQUIRE(mibi_bytes(8) == 8ULL * 1024ULL * 1024ULL);
  STATIC_REQUIRE(gibi_bytes(2) == 2ULL * 1024ULL * 1024ULL * 1024ULL);
}

TEST_CASE("constexpr functions also work at runtime", "[memory]") {
  unsigned long long n = 3;
  REQUIRE(kilo_bytes(n) == 3ULL * 1000ULL);
  REQUIRE(mega_bytes(n) == 3ULL * 1000ULL * 1000ULL);
  REQUIRE(giga_bytes(n) == 3ULL * 1000ULL * 1000ULL * 1000ULL);
  REQUIRE(kibi_bytes(n) == 3ULL * 1024ULL);
  REQUIRE(mibi_bytes(n) == 3ULL * 1024ULL * 1024ULL);
  REQUIRE(gibi_bytes(n) == 3ULL * 1024ULL * 1024ULL * 1024ULL);
}
