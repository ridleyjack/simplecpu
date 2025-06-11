#pragma once

#include "CPUDefs.h"
#include "Nibble.h"

namespace alu {

struct Flags {
  bool Overflow{};
  bool Zero{};
  bool Negative{};

  void Clear();
};

class ALU {
public:
  ALU(cpu::Register& result);

  void DoOperation(cpu::uint4 inputA, cpu::uint4 inputB, cpu::OpCode op);

  const Flags& GetFlags() const;

private:
  cpu::Register& m_result;
  Flags m_flags{};

  void m_add(cpu::uint4 inputA, cpu::uint4 inputB);
  void m_sub(cpu::uint4 inputA, cpu::uint4 inputB);
};

} // namespace alu
