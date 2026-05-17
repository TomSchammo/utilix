#ifndef MEMORY_HPP
#define MEMORY_HPP

namespace utilix {

[[nodiscard]] constexpr unsigned long long kilo_bytes(unsigned long long n) {
  return n * 1024ULL;
}

template <unsigned long long n>
[[nodiscard]] constexpr unsigned long long kilo_bytes() {
  return n * 1024ULL;
}

[[nodiscard]] constexpr unsigned long long mega_bytes(unsigned long long n) {
  return n * 1024ULL * 1024ULL;
}

template <unsigned long long n>
[[nodiscard]] constexpr unsigned long long mega_bytes() {
  return n * 1024ULL * 1024ULL;
}

[[nodiscard]] constexpr unsigned long long giga_bytes(unsigned long long n) {
  return n * 1024ULL * 1024ULL * 1024ULL;
}

template <unsigned long long n>
[[nodiscard]] constexpr unsigned long long giga_bytes() {
  return n * 1024ULL * 1024ULL * 1024ULL;
}

} // namespace utilix

#endif // MEMORY_HPP
