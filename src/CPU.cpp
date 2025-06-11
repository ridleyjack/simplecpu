#include "CPU.h"

#include "CPUDefs.h"
#include "Nibble.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <utility>

namespace cpu {

// m_parse2Args takes in a 4 bit number and splits it into two "2 bit" numbers.
// Example: A binary number 0000 would have args stored as AABB.
// They would be split into two args {00AA, 00BB}.
// Note that there is no two bit custom type.
std::array<uint4, 2> CPU::m_parse2Args(const uint4 value) {

  constexpr uint4 highMask{0x03 << 2}; // 1100
  constexpr uint4 lowMask{0x03};       // 0011

  const uint4 arg0 = (value & highMask) >> 2;
  const uint4 arg1 = (value & lowMask);
  return {arg0, arg1};
}

CPU::CPU() : m_alu{m_aluResult}, m_memory{cpu::MemSizeInWords} {}

void CPU::Run() {
  while (!m_halt) {
    CPU::Cycle();
  }
}

// Cycle reads the next instruction from RAM and executes it fully.
// Instructions that use more than one line of memory will read in the
// additional number of required lines.
void CPU::Cycle() {
  // Fetch.
  m_IS = m_memory.Load(m_PC++);

  // Decode.
  const OpCode op{static_cast<uint8_t>(m_IS)};

  // Execute.
  std::array<uint4, 2> args{}; // Some instructions take two 2 bit arguments.
  uint4& arg0 = args[0];       // Others only use a single 4 bit argument.

  switch (op) {
  case OpCode::LoadA:
    arg0 = m_memory.Load(m_PC++);
    m_loadRegister(regID::A, arg0);
    break;

  case OpCode::LoadB:
    arg0 = m_memory.Load(m_PC++);
    m_loadRegister(regID::B, arg0);
    break;

  case OpCode::StoreA:
    arg0 = m_memory.Load(m_PC++);
    m_storeRegister(regID::A, arg0);
    break;

  case OpCode::Mov:
    args = m_parse2Args(m_memory.Load(m_PC++));
    m_moveRegister(args[0], args[1]);
    break;

  case OpCode::Add:
    args = m_parse2Args(m_memory.Load(m_PC++));
    m_aluOperation(args[0], args[1], OpCode::Add);
    m_registers[regID::A] = m_aluResult;
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
    if (m_alu.GetFlags().Zero) {
      m_PC = arg0;
    }
    break;

  case OpCode::JumpNZ:
    arg0 = m_memory.Load(m_PC++);
    if (!m_alu.GetFlags().Zero) {
      m_PC = arg0;
    }
    break;

  case OpCode::Halt:
    m_halt = true;
    break;

  default:
    std::cerr << "Unknown opcode: " << std::to_underlying(op) << '\n';
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

void CPU::m_loadRegister(const size_t regID, const uint4 address) {
  const auto regA = static_cast<uint8_t>(regID);
  m_registers[regA] = m_memory.Load(address);
}

void CPU::m_storeRegister(const size_t regID, const uint4 address) {
  const auto regA = static_cast<uint8_t>(regID);
  m_memory.Store(m_registers[regA], address);
}

void CPU::m_moveRegister(const uint4 srcID, const uint4 destID) {
  const auto src = static_cast<uint8_t>(srcID);
  const auto dest = static_cast<uint8_t>(destID);
  m_registers[dest] = m_registers[src];
}

void CPU::m_aluOperation(const uint4 inputA, const uint4 inputB,
                         const OpCode op) {
  const auto regA = static_cast<uint8_t>(inputA);
  const auto regB = static_cast<uint8_t>(inputB);
  m_alu.DoOperation(m_registers[regA], m_registers[regB], op);
}

} // namespace cpu