#include "CPU.h"

#include <iostream>
#include <utility>

namespace cpu {

CPU::CPU() : m_alu{m_aluResult}, m_memory{16} {}
void CPU::Run() {
  while (!m_halt) {
    CPU::Cycle();
  }
}

void CPU::Cycle() {
  // Fetch.
  m_IS = m_memory.Load(m_PC);
  m_PC++;

  // Decode.
  constexpr cpu::uint_t opMask{0xF0};
  cpu::uint_t op = (m_IS & opMask) >> 4;

  // Some instructions specify an address/value via the operand.
  constexpr cpu::uint_t valueMask{0x0F};
  cpu::uint_t value = (m_IS & valueMask);

  // Some instructions specify the registers via operands.
  Register regA = m_operandA(value);
  Register regB = m_operandB(value);

  // Execute.
  switch (static_cast<OpCode>(op)) {
  case OpCode::LoadA:
    m_loadRegister(m_registers[0], value);
    break;

  case OpCode::LoadB:
    m_loadRegister(m_registers[1], value);
    break;

  case OpCode::StoreA:
    m_storeRegister(m_registers[0], value);
    break;

  case OpCode::Mov:
    if (regA == RegUnspecified || regB == RegUnspecified) {
      std::cout << "Mov requires two registers to be specified via operands";
      m_halt = true;
      return;
    }
    m_moveRegister(m_registers[regA], m_registers[regB]);
    break;

  case OpCode::Add:
    if (regA == RegUnspecified || regB == RegUnspecified) {
      std::cout << "Add requires two registers to be specified via operands";
      m_halt = true;
      return;
    }
    m_aluOperation(m_registers[regA], m_registers[regB], OpCode::Add);
    m_registers[0] = m_aluResult;
    break;

  case OpCode::Sub:
    if (regA == RegUnspecified || regB == RegUnspecified) {
      std::cout << "Sub requires two registers to be specified via operands";
      m_halt = true;
      return;
    }
    m_aluOperation(m_registers[regA], m_registers[regB], OpCode::Sub);
    m_registers[0] = m_aluResult;
    break;

  case OpCode::Jump:
    m_PC = value;
    break;

  case OpCode::JumpNeg:
    if (m_alu.GetFlags().negative) {
      m_PC = value;
    }
    break;

  default:
    std::cout << "Unknown opcode: " << static_cast<uint>(op) << '\n';
  case OpCode::Halt:
    m_halt = true;
    break;
  }
}
Register CPU::GetRegisterA() const {
  return m_registers[0];
}

Register CPU::GetRegisterB() const {
  return m_registers[1];
}

const alu::Flags& CPU::GetFlags() const {
  return m_alu.GetFlags();
}

Memory& CPU::GetMemory() {
  return m_memory;
}

void CPU::m_loadRegister(Register& reg, uint_t address) {
  reg = m_memory.Load(address);
}

void CPU::m_storeRegister(Register reg, uint_t address) {
  m_memory.Store(reg, address);
}
void CPU::m_moveRegister(Register src, Register& dest) {
  dest = src;
}

void CPU::m_aluOperation(Register inputA, Register inputB, OpCode op) {
  m_alu.DoOperation(inputA, inputB, op);
}

Register CPU::m_operandA(uint inst) {
  if (inst & 0x04) {
    return 0;
  } else if (inst & 0x08) {
    return 1;
  }
  return RegUnspecified;
}

Register CPU::m_operandB(uint inst) {
  if (inst & 0x01) {
    return 0;
  } else if (inst & 0x02) {
    return 1;
  }
  return RegUnspecified;
}
} // namespace cpu