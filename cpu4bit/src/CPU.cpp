#include "CPU.h"

#include <iostream>

namespace cpu {
std::array<uint_t, 2> CPU::m_parse2Args(uint_t value) {
  constexpr uint_t highMask{0x03 << 2}; // 1100
  constexpr uint_t lowMask{0x03};       // 0011

  const uint_t arg0 = (value & highMask) >> 2;
  const uint_t arg1 = (value & lowMask);
  return {arg0, arg1};
}

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
  auto op{static_cast<OpCode>(m_IS)};

  // Execute.
  std::array<uint_t, 2> args{};
  uint_t& arg0 = args[0];

  switch (op) {
  case OpCode::LoadA:
    arg0 = m_memory.Load(m_PC++);
    m_loadRegister(0, arg0);
    break;

  case OpCode::LoadB:
    arg0 = m_memory.Load(m_PC++);
    m_loadRegister(1, arg0);
    break;

  case OpCode::StoreA:
    arg0 = m_memory.Load(m_PC++);
    m_storeRegister(0, arg0);
    break;

  case OpCode::Mov:
    args = m_parse2Args(m_memory.Load(m_PC++));
    m_moveRegister(args[0], args[1]);
    break;

  case OpCode::Add:
    args = m_parse2Args(m_memory.Load(m_PC++));
    m_aluOperation(args[0], args[1], OpCode::Add);
    m_registers[0] = m_aluResult;
    break;

  case OpCode::Sub:
    args = m_parse2Args(m_memory.Load(m_PC++));
    m_aluOperation(args[0], args[1], OpCode::Sub);
    m_registers[0] = m_aluResult;
    break;

  case OpCode::Jump:
    arg0 = m_memory.Load(m_PC);
    m_PC = arg0;
    break;

  case OpCode::JumpZ:
    arg0 = m_memory.Load(m_PC++);
    if (m_alu.GetFlags().zero) {
      m_PC = arg0;
    }
    break;

  case OpCode::JumpNZ:
    arg0 = m_memory.Load(m_PC++);
    if (!m_alu.GetFlags().zero) {
      m_PC = arg0;
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

void CPU::m_loadRegister(uint_t regID, uint_t address) {
  m_registers[regID] = m_memory.Load(address);
}

void CPU::m_storeRegister(uint_t regID, uint_t address) {
  m_memory.Store(m_registers[regID], address);
}

void CPU::m_moveRegister(uint_t srcID, uint_t destID) {
  m_registers[destID] = m_registers[srcID];
}

void CPU::m_aluOperation(uint_t inputA, uint_t inputB, OpCode op) {
  m_alu.DoOperation(m_registers[inputA], m_registers[inputB], op);
}

} // namespace cpu