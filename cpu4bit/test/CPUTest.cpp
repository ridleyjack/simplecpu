#include <cassert>

#include "CPU.h"
#include "CPUDefs.h"
#include "Memory.h"

#include <iostream>
#include <utility>

namespace cpu {
// Helper functions to make instructions.

uint_t twoArgs(uint_t arg0, uint_t arg1) {
  arg0 = arg0 << 2;
  return arg0 | arg1;
}

// Test various functionality.

void testStore() {
  auto* cpu = new cpu::CPU();
  Memory& mem{cpu->GetMemory()};
  uint addr{0};

  mem.Store(4, 14);
  // A = 26;
  mem.Store(std::to_underlying(OpCode::LoadA), addr++);
  mem.Store(14, addr++);
  // mem[15] = A;
  mem.Store(std::to_underlying(OpCode::StoreA), addr++);
  mem.Store(15, addr);

  assert(cpu->GetMemory().Load(15) == 0);
  cpu->Run();
  assert(cpu->GetMemory().Load(15) == 4);
}

void testMove() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();
  mem.Store(15, 15);

  // B = 15;
  mem.Store(std::to_underlying(OpCode::LoadB), 0);
  mem.Store(15, 1);
  // A = B;
  mem.Store(std::to_underlying(OpCode::Mov), 2);
  mem.Store(twoArgs(1, 0), 3);

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 0);
  cpu->Run();
  result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 15);
}

void testMove2() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();
  mem.Store(15, 15);

  // A = 15;
  mem.Store(std::to_underlying(OpCode::LoadA), 0);
  mem.Store(15, 1);
  // B = A;
  mem.Store(std::to_underlying(OpCode::Mov), 2);
  mem.Store(twoArgs(0, 1), 3);

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterB());
  assert(result == 0);
  cpu->Run();
  result = static_cast<cpu::int_t>(cpu->GetRegisterB());
  assert(result == 15);
}

void testAdd() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();
  mem.Store(2, 14);
  mem.Store(4, 15);

  // A = 2
  mem.Store(std::to_underlying(OpCode::LoadA), 0);
  mem.Store(14, 1);
  // B = 4
  mem.Store(std::to_underlying(OpCode::LoadB), 2);
  mem.Store(15, 3);
  // A = A + B = 6
  mem.Store(std::to_underlying(OpCode::Add), 4);
  mem.Store(twoArgs(0, 1), 5);

  cpu->Run();

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 6);

  delete cpu;
}

void testSub() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();
  mem.Store(15, 14);
  mem.Store(5, 15);

  // A = 15
  mem.Store(std::to_underlying(OpCode::LoadA), 0);
  mem.Store(14, 1);
  // B = 5
  mem.Store(std::to_underlying(OpCode::LoadB), 2);
  mem.Store(15, 3);
  // A = A - B = 10
  mem.Store(std::to_underlying(OpCode::Sub), 4);
  mem.Store(twoArgs(0, 1), 5);

  cpu->Run();

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 10);

  delete cpu;
}

void testSub2() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();
  mem.Store(15, 14);
  mem.Store(5, 15);

  // A = 15
  mem.Store(std::to_underlying(OpCode::LoadA), 0);
  mem.Store(14, 1);
  // B = 5
  mem.Store(std::to_underlying(OpCode::LoadB), 2);
  mem.Store(15, 3);
  // A = B - A = -10
  mem.Store(std::to_underlying(OpCode::Sub), 4);
  mem.Store(twoArgs(1, 0), 5);

  cpu->Run();

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == -10);

  delete cpu;
}

void testHalt() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();

  mem.Store(8, 14);
  mem.Store(5, 15);

  uint_t addr = 0;
  // A = 8;
  mem.Store(std::to_underlying(OpCode::LoadA), addr++);
  mem.Store(14, addr++);
  // Halt.
  mem.Store(std::to_underlying(OpCode::Halt), addr++);
  // A = 5;
  mem.Store(std::to_underlying(OpCode::LoadA), addr++);
  mem.Store(15, addr++);

  cpu->Run();
  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 8);

  delete cpu;
}

void testJump() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();

  mem.Store(1, 14);
  mem.Store(10, 15);

  // A = 1
  mem.Store(std::to_underlying(OpCode::LoadA), 0);
  mem.Store(14, 1);
  // B = 10
  mem.Store(std::to_underlying(OpCode::LoadB), 2);
  mem.Store(15, 3);
  // jump addr:7
  mem.Store(std::to_underlying(OpCode::Jump), 4);
  mem.Store(7, 5);
  // Halt (skipped)
  mem.Store(std::to_underlying(OpCode::Halt), 6);

  // A = A+B = 11
  mem.Store(std::to_underlying(OpCode::Add), 7);
  mem.Store(twoArgs(0, 1), 8);

  cpu->Run();

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 11);

  delete cpu;
}

void testJumpZero() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();

  mem.Store(5, 14);
  mem.Store(11, 15);

  // A = 5
  mem.Store(std::to_underlying(OpCode::LoadA), 0);
  mem.Store(14, 1);
  // B = 5
  mem.Store(std::to_underlying(OpCode::LoadB), 2);
  mem.Store(14, 3);
  // A = A-B = 0
  mem.Store(std::to_underlying(OpCode::Sub), 4);
  mem.Store(twoArgs(0, 1), 5);
  // Jump over Halt if ALU result == 0
  mem.Store(std::to_underlying(OpCode::JumpZ), 6);
  mem.Store(9, 7);
  // Halt (skipped)
  mem.Store(std::to_underlying(OpCode::Halt), 8);
  // A = 11
  mem.Store(std::to_underlying(OpCode::LoadA), 9);
  mem.Store(15, 10);

  cpu->Run();

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 11);
}

void testJumpNotZero() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();

  mem.Store(3, 14);
  mem.Store(1, 15);

  // A = 3
  mem.Store(std::to_underlying(OpCode::LoadA), 0);
  mem.Store(14, 1);
  // B = 1
  mem.Store(std::to_underlying(OpCode::LoadB), 2);
  mem.Store(14, 3);
  // A = A-B
  mem.Store(std::to_underlying(OpCode::Sub), 4);
  mem.Store(twoArgs(0, 1), 5);
  // Jump to previous if A > 0;
  mem.Store(std::to_underlying(OpCode::JumpNZ), 6);
  mem.Store(5, 7);

  cpu->Run();

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 0);
}

} // namespace cpu

void RunAllCPUTests() {
  cpu::testStore();
  cpu::testMove();
  cpu::testMove2();
  cpu::testAdd();
  cpu::testSub();
  cpu::testSub2();

  cpu::testHalt();

  cpu::testJump();
  cpu::testJumpZero();
  cpu::testJumpNotZero();
}