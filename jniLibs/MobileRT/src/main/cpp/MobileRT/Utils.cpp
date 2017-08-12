//
// Created by Tiago on 14-11-2016.
//

#include "Utils.hpp"

int roundDownToMultipleOf(const int value, const int multiple) noexcept {
    const int rest(value % multiple);
    return rest > 0 ? value - rest : value;
}

//https://en.wikipedia.org/wiki/Halton_sequence
float haltonSequence (uint32_t index, const unsigned int base) noexcept {
  float f {1.0f};
  float result {0.0f};
  while (index > 0) {
    f /= base;
    result += f * (index % base);
    index = static_cast<uint32_t> (std::floor (index / base));
  }
  return result;
}
