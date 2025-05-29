#include "ALU.h"

#include <iostream>
#include <limits>
#include <utility>

namespace alu {

void Flags::Clear() {
  negative = false;
  zero = false;
  overflow = false;
}

ALU::ALU(cpu::Register& dest) : m_destination(dest) {}

void ALU::DoOperation(cpu::Register registerA, cpu::Register registerB,
                      cpu::OpCode op) {
  m_flags.Clear();

  switch (op) {
  case cpu::OpCode::Add:
    m_addOrSub(true, registerA, registerB);
    break;
  case cpu::OpCode::Sub:
    m_addOrSub(false, registerA, registerB);
    break;
  default:
    // No op if it's an OpCode we don't support.
    std::cerr << "Unknown ALU operation: " << static_cast<int>(op) << std::endl;
  }
}

const Flags& ALU::GetFlags() const {
  return m_flags;
}

void ALU::m_addOrSub(bool addition, cpu::Register regA, cpu::Register regB) {
  auto const inputA = static_cast<cpu::int_t>(regA);
  auto const inputB = static_cast<cpu::int_t>(regB);

  cpu::int_t result;
  if (addition) {
    m_flags.overflow = __builtin_add_overflow(inputA, inputB, &result);
  } else {
    m_flags.overflow = __builtin_sub_overflow(inputA, inputB, &result);
  }

  if (result == 0) {
    m_flags.zero = true;
  }
  if (result < 0) {
    m_flags.negative = true;
  }
  m_destination = result;
}

} // namespace alu