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

[[nodiscard]] constexpr unsigned long long mega_bytes(unsigned long long n) {
  return n * MB;
}

[[nodiscard]] constexpr unsigned long long giga_bytes(unsigned long long n) {
  return n * GB;
}

[[nodiscard]] constexpr unsigned long long kibi_bytes(unsigned long long n) {
  return n * KiB;
}

[[nodiscard]] constexpr unsigned long long mibi_bytes(unsigned long long n) {
  return n * MiB;
}

[[nodiscard]] constexpr unsigned long long gibi_bytes(unsigned long long n) {
  return n * GiB;
}

} // namespace utilix::memory

#endif // UTILIX_MEMORY_HPP
