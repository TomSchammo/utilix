#ifndef UTILIX_TIMING_HPP
#define UTILIX_TIMING_HPP

#include "types.hpp"
#include <chrono>
#include <format>
#include <iostream>
#include <type_traits>

namespace utilix::timing {

template <typename T>
concept ChronoDuration = requires {
  requires std::is_same_v<T, std::chrono::duration<typename T::rep, typename T::period>>;
};

template <ChronoDuration precision>
static consteval std::string_view time_str() {
  if constexpr (std::is_same_v<precision, std::chrono::milliseconds>)
    return "ms";
  else if constexpr (std::is_same_v<precision, std::chrono::seconds>)
    return "s";
  else if constexpr (std::is_same_v<precision, std::chrono::microseconds>)
    return "us";
  else if constexpr (std::is_same_v<precision, std::chrono::nanoseconds>)
    return "ns";
  else if constexpr (std::is_same_v<precision, std::chrono::minutes>)
    return "min";
  else
    throw std::format("Duration {} is not supported");
}

template <ChronoDuration precision = std::chrono::milliseconds>
class ScopedVTimer {
  std::string name_;
  std::chrono::high_resolution_clock::time_point start_;
  utilix::types::u64& duration_ref_;

public:
  ScopedVTimer(const std::string& name, utilix::types::u64& duration_ref)
      : name_(name), start_(std::chrono::high_resolution_clock::now()),
        duration_ref_(duration_ref) {}

  ScopedVTimer(const ScopedVTimer&) = delete;
  ScopedVTimer(ScopedVTimer&&) = delete;
  ScopedVTimer& operator=(const ScopedVTimer&) = delete;
  ScopedVTimer& operator=(ScopedVTimer&&) = delete;

  ~ScopedVTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<precision>(end - start_);

    duration_ref_ = static_cast<utilix::types::u64>(duration.count());

    std::cout << name_ << " took " << duration << "\n";
  }
};

template <ChronoDuration precision = std::chrono::milliseconds>
class ScopedTimer {
  std::string name_;
  std::chrono::high_resolution_clock::time_point start_;

public:
  explicit ScopedTimer(const std::string& name)
      : name_(name), start_(std::chrono::high_resolution_clock::now()) {}

  ScopedTimer(const ScopedTimer&) = delete;
  ScopedTimer(ScopedTimer&&) = delete;
  ScopedTimer& operator=(const ScopedTimer&) = delete;
  ScopedTimer& operator=(ScopedTimer&&) = delete;

  ~ScopedTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start_;

    std::cout << name_ << " took " << std::chrono::duration_cast<precision>(duration) << "\n";
  }
};

class ScopedTimerMs {
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;

public:
  ScopedTimerMs() : start_(std::chrono::high_resolution_clock::now()) {}

  ScopedTimerMs(const ScopedTimerMs&) = default;
  ScopedTimerMs(ScopedTimerMs&&) = delete;
  ScopedTimerMs& operator=(const ScopedTimerMs&) = default;
  ScopedTimerMs& operator=(ScopedTimerMs&&) = delete;

  ~ScopedTimerMs() {
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = end - start_;

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration)
              << " have elapsed\n";
  }
};

class ScopedTimerS {
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;

public:
  ScopedTimerS() : start_(std::chrono::high_resolution_clock::now()) {}

  ScopedTimerS(const ScopedTimerS&) = default;
  ScopedTimerS(ScopedTimerS&&) = delete;
  ScopedTimerS& operator=(const ScopedTimerS&) = default;
  ScopedTimerS& operator=(ScopedTimerS&&) = delete;

  ~ScopedTimerS() {
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = end - start_;

    std::cout << std::chrono::duration_cast<std::chrono::seconds>(duration) << " have elapsed\n";
  }
};

class ScopedTimerM {
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;

public:
  ScopedTimerM() : start_(std::chrono::high_resolution_clock::now()) {}

  ScopedTimerM(const ScopedTimerM&) = default;
  ScopedTimerM(ScopedTimerM&&) = delete;
  ScopedTimerM& operator=(const ScopedTimerM&) = default;
  ScopedTimerM& operator=(ScopedTimerM&&) = delete;

  ~ScopedTimerM() {
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = end - start_;

    std::cout << std::chrono::duration_cast<std::chrono::minutes>(duration) << " have elapsed\n";
  }
};

} // namespace utilix::timing

#endif // UTILIX_TIMING_HPP
