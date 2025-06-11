#include "ALU.h"

#include "CPUDefs.h"
#include "Nibble.h"

#include <iostream>

namespace alu {

void Flags::Clear() {
  Negative = false;
  Zero = false;
  Overflow = false;
}

ALU::ALU(cpu::Register& result) : m_result(result) {}

void ALU::DoOperation(const cpu::uint4 inputA, const cpu::uint4 inputB,
                      const cpu::OpCode op) {
  m_flags.Clear();

  switch (op) {
  case cpu::OpCode::Add:
    m_add(inputA, inputB);
    break;
  case cpu::OpCode::Sub:
    m_sub(inputA, inputB);
    break;
  default:
    // No op if it's an OpCode we don't support.
    std::cerr << "ALU: Unknown OpCode: " << static_cast<uint>(op) << '\n';
  }
}

const Flags& ALU::GetFlags() const {
  return m_flags;
}

void ALU::m_add(const cpu::uint4 inputA, const cpu::uint4 inputB) {
  m_result = 0;

  // Full adder ripple style addition.
  bool carry = false;
  for (int i = 0; i < 4; i++) {
    const bool bitA = ((inputA >> i) & 1) == 1;
    const bool bitB = ((inputB >> i) & 1) == 1;

    const bool sum = static_cast<bool>(bitA ^ bitB ^ carry);
    carry = (bitA & bitB) | (bitA & carry) | (bitB & carry);

    m_result |= sum << i;
  }

  // Check for overflow and set flags.
  const bool signA = ((inputA >> 3) & 1) == 1;
  const bool signB = ((inputB >> 3) & 1) == 1;
  const bool signR = ((m_result >> 3) & 1) == 1;

  // Overflow if two positive numbers give a negative, or two negative numbers
  // a positive.
  // A positive and negative number can never overflow so no check for that.
  if ((signA == signB) && (signA != signR)) {
    m_flags.Overflow = true;
  }
  if (signR) { // Twos compliment binary: 1xxx == negative, 0xxx = positive.
    m_flags.Negative = true;
  }
  if (m_result == 0) {
    m_flags.Zero = true;
  }
}
void ALU::m_sub(const cpu::uint4 inputA, cpu::uint4 inputB) {
  // Subtraction is done via addition: A - B == A + (-B)
  // -B is found by taking ~B + 1 in 2s compliment.

  inputB = ~inputB + cpu::uint4(1);
  m_add(inputA, inputB);
}

} // namespace alu