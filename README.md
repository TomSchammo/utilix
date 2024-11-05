
# Utilitx

This is a small header-only library I made for personal projects.

There are no requirements other than C++14 and CMake 3.18.

It can be installed by running `sudo make install` in the project root.

## Components


1. `types.hpp`

`types.hpp` contains a few type-aliases for common numeric types such as `std::uint32_t` -> `u32` or `std::int64_t` -> `i64`.

2. `timing.hpp`

`timing.hpp` contains a few classes for code timing purposes.
They are scoped timers which meaning they rely on object lifetimes to determine the runtime of code.

Example:

```cpp
#include <chrono>
#include <thread>
#include <utilix/timing.hpp>

int main (int argc, char *argv[]) {
    {
        utilix::ScopedTimerMs t;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
```

resulting in the following output

```
> g++ test.cpp -o t -std=c++14 && ./t
2005ms have elapsed
```
