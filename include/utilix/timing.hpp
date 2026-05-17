

#ifndef TIMING_HPP
#define TIMING_HPP

#include "types.hpp"
#include <chrono>
#include <iostream>

namespace utilix {

template <typename precision = std::chrono::milliseconds> struct ScopedTimer {
private:
  std::string name;
  std::chrono::high_resolution_clock::time_point start;

  constexpr std::string_view time_str() {
    if (typeid(precision) == typeid(std::chrono::milliseconds))
      return "ms";
    else if (typeid(precision) == typeid(std::chrono::seconds))
      return "s";
    else if (typeid(precision) == typeid(std::chrono::microseconds))
      return "us";
    else if (typeid(precision) == typeid(std::chrono::nanoseconds))
      return "ns";
    else if (typeid(precision) == typeid(std::chrono::minutes))
      return "min";
    else
      return "";
  }

public:
  ScopedTimer(const std::string &name)
      : name(name), start(std::chrono::high_resolution_clock::now()) {}

  ~ScopedTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;

    std::cout << name << " took "
              << std::chrono::duration_cast<precision>(duration).count()
              << time_str() << "\n";
  }
};

struct ScopedTimerMs {
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::chrono::duration<f64> duration;

  ScopedTimerMs() { start = std::chrono::high_resolution_clock::now(); }

  ~ScopedTimerMs() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration)
                     .count()
              << "ms have elapsed\n";
  }
};

struct ScopedTimerS {
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::chrono::duration<f64> duration;

  ScopedTimerS() { start = std::chrono::high_resolution_clock::now(); }

  ~ScopedTimerS() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout
        << std::chrono::duration_cast<std::chrono::seconds>(duration).count()
        << "s have elapsed\n";
  }
};

struct ScopedTimerM {
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::chrono::duration<f64> duration;

  ScopedTimerM() { start = std::chrono::high_resolution_clock::now(); }

  ~ScopedTimerM() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout
        << std::chrono::duration_cast<std::chrono::minutes>(duration).count()
        << "minutes have elapsed\n";
  }
};

} // namespace utilix

#endif // !TIMING_HPP
