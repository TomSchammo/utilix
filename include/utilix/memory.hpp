#ifndef UTILIX_MEMORY_HPP
#define UTILIX_MEMORY_HPP

namespace utilix::memory {

constexpr auto KiB = 1024ULL;
constexpr auto MiB = KiB * KiB;
constexpr auto GiB = KiB * KiB * KiB;

constexpr auto KB = 1000ULL;
constexpr auto MB = KB * KB;
constexpr auto GB = KB * KB * KB;

[[nodiscard]] constexpr unsigned long long kilo_bytes(unsigned long long n) {
  return n * KB;
}

template <unsigned long long n>
[[nodiscard]] constexpr unsigned long long kilo_bytes() {
  return n * KB;
}

[[nodiscard]] constexpr unsigned long long mega_bytes(unsigned long long n) {
  return n * MB;
}

template <unsigned long long n>
[[nodiscard]] constexpr unsigned long long mega_bytes() {
  return n * MB;
}

[[nodiscard]] constexpr unsigned long long giga_bytes(unsigned long long n) {
  return n * GB;
}

template <unsigned long long n>
[[nodiscard]] constexpr unsigned long long giga_bytes() {
  return n * GB;
}

} // namespace utilix::memory

#endif // UTILIX_MEMORY_HPP
