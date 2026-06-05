#ifndef UTILIX_TIMING_HPP
#define UTILIX_TIMING_HPP

#include "types.hpp"
#include <chrono>
#include <format>
#include <iostream>
#include <type_traits>

namespace utilix {
namespace timing {

template <typename T>
concept ChronoDuration = requires {
  requires std::is_same_v<
      T, std::chrono::duration<typename T::rep, typename T::period>>;
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
struct ScopedVTimer {
private:
  std::string name_;
  std::chrono::high_resolution_clock::time_point start_;
  utilix::types::u64& duration_ref_;

public:
  ScopedVTimer(const std::string& name, utilix::types::u64& duration_ref)
      : name_(name), start_(std::chrono::high_resolution_clock::now()),
        duration_ref_(duration_ref) {}

  ~ScopedVTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<precision>(end - start_);

    duration_ref_ = static_cast<utilix::types::u64>(duration.count());

    std::cout << name_ << " took " << duration << "\n";
  }
};

template <ChronoDuration precision = std::chrono::milliseconds>
struct ScopedTimer {
private:
  std::string name_;
  std::chrono::high_resolution_clock::time_point start_;

public:
  explicit ScopedTimer(const std::string& name)
      : name_(name), start_(std::chrono::high_resolution_clock::now()) {}

  ~ScopedTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start_;

    std::cout << name_ << " took " << std::chrono::duration_cast<precision>(duration) << "\n";
  }
};

struct ScopedTimerMs {
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::chrono::duration<types::f64> duration;

  ScopedTimerMs() : start(std::chrono::high_resolution_clock::now()) {}

  ~ScopedTimerMs() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration)
              << " have elapsed\n";
  }
};

struct ScopedTimerS {
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::chrono::duration<types::f64> duration;

  ScopedTimerS() : start(std::chrono::high_resolution_clock::now()) {}

  ~ScopedTimerS() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout << std::chrono::duration_cast<std::chrono::seconds>(duration) << " have elapsed\n";
  }
};

struct ScopedTimerM {
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::chrono::duration<types::f64> duration;

  ScopedTimerM() : start(std::chrono::high_resolution_clock::now()) {}

  ~ScopedTimerM() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout << std::chrono::duration_cast<std::chrono::minutes>(duration) << " have elapsed\n";
  }
};

} // namespace timing
} // namespace utilix

#endif // UTILIX_TIMING_HPP
