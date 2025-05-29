#pragma once

#include <array>

#include "ALU.h"
#include "CPUDefs.h"
#include "Memory.h"

#include <sys/types.h>

namespace cpu {
class CPU {
public:
  CPU();

  void Run();
  void Cycle();

  Register GetRegisterA() const;
  Register GetRegisterB() const;

  const alu::Flags& GetFlags() const;
  Memory& GetMemory();

private:
  static Register m_operandA(uint inst);
  static Register m_operandB(uint inst);

  alu::ALU m_alu;
  Memory m_memory;
  std::array<Register, 2> m_registers{};

  Register m_IS{};        // Instruction store.
  Register m_PC{};        // Program counter.
  Register m_aluResult{}; // aluResult buffers the output of the ALU.

  void m_loadRegister(Register& reg, uint_t address);
  void m_storeRegister(Register reg, uint_t address);
  void m_moveRegister(Register src, Register& dest);

  void m_aluOperation(Register inputA, Register inputB, OpCode op);

  bool m_halt{};
};

} // namespace cpu