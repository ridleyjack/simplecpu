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
  static std::array<uint_t, 2> m_parse2Args(uint_t value);

  alu::ALU m_alu;
  Memory m_memory;
  std::array<Register, 2> m_registers{};

  Register m_IS{};        // Instruction store.
  Register m_PC{};        // Program counter.
  Register m_aluResult{}; // aluResult buffers the output of the ALU.

  void m_loadRegister(uint_t regID, uint_t address);
  void m_storeRegister(uint_t regID, uint_t address);
  void m_moveRegister(uint_t srcID, uint_t destID);

  void m_aluOperation(uint_t inputA, uint_t inputB, OpCode op);

  bool m_halt{};
};

} // namespace cpu