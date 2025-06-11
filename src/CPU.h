#pragma once

#include <array>

#include "ALU.h"
#include "CPUDefs.h"
#include "Memory.h"
#include "Nibble.h"

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
  static std::array<uint4, 2> m_parse2Args(uint4 value);

  alu::ALU m_alu;
  Memory m_memory;
  std::array<Register, NumRegisters> m_registers{};

  Register m_IS{};        // Instruction store.
  Register m_PC{};        // Program counter.
  Register m_aluResult{}; // aluResult buffers the output of the ALU.

  void m_loadRegister(size_t regID, uint4 address);
  void m_storeRegister(size_t regID, uint4 address);
  void m_moveRegister(uint4 srcID, uint4 destID);

  void m_aluOperation(uint4 inputA, uint4 inputB, OpCode op);

  bool m_halt{};
};

} // namespace cpu