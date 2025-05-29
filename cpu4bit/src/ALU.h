#pragma once

#include <array>

#include "CPUDefs.h"

namespace alu {

struct Flags {
  bool overflow{};
  bool zero{};
  bool negative{};

  void Clear();
};

class ALU {
public:
  ALU(cpu::Register& dest);

  void DoOperation(cpu::Register registerA, cpu::Register registerB,
                   cpu::OpCode op);

  const Flags& GetFlags() const;

private:
  cpu::Register& m_destination;
  Flags m_flags{};

  void m_addOrSub(bool addition, cpu::Register regA, cpu::Register regB);
};

} // namespace alu
