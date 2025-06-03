#pragma once
#include <cstdint>
#include <limits>

namespace cpu {

using int_t = std::int8_t;
using uint_t = std::uint8_t;

using Register = uint_t;
constexpr Register RegUnspecified{std::numeric_limits<Register>::max()};

enum class OpCode : uint_t {
  Halt = 0,
  LoadA = 1,
  LoadB = 2,
  StoreA = 3,
  Mov = 4,
  Add = 5,
  Sub = 6,
  Jump = 7,
  JumpZ = 8,
  JumpNZ = 9,
};

} // namespace cpu
