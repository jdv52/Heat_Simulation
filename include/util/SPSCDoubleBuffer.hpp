#pragma once

#include <array>

namespace HeatSim {

template <typename T> class SPSCDoubleBuffer {

public:
  SPSCDoubleBuffer() : buff{}, front(&buff[0]), back(&buff[1]) {}

  const T &read() {
    // consume
    return *front;
  }

  void write(T val) {
    *back = val;
    std::swap(front, back);

    // release
  }

private:
  std::array<T, 2> buff;
  T *front;
  T *back;
};

} // namespace HeatSim
