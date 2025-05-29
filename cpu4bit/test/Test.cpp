#include "Test.h"

#include <cassert>

#include "CPU.h"
#include "CPUDefs.h"
#include "Memory.h"

// Helper functions to make instructions.

// 4bit opCode, 4bit value.
cpu::uint_t oneOperand(cpu::OpCode op, cpu::uint_t value) {
  auto result = static_cast<cpu::uint_t>(op);
  result = result << 4;
  result = result | value;
  return result;
}

// 4bit OpCode, 2bit value, 2bit value.
cpu::uint_t twoOperand(cpu::OpCode op, cpu::uint_t inA, cpu::uint_t inB) {
  auto result = static_cast<cpu::uint_t>(op);
  result = result << 4;
  inA = inA << 2;
  result = result | inA | inB;
  return result;
}

// Test various functionality.

void testAddNumbers() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();

  mem.Store(2, 14);
  mem.Store(4, 15);

  // A = 2
  cpu::uint_t inst = oneOperand(cpu::OpCode::LoadA, 14);
  mem.Store(inst, 0);
  // B = 4
  inst = oneOperand(cpu::OpCode::LoadB, 15);
  mem.Store(inst, 1);
  // A = A + B
  inst = twoOperand(cpu::OpCode::Add, 0x01, 0x02);
  mem.Store(inst, 2);

  cpu->Run();

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 6);
}

void testJump() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();

  mem.Store(1, 14);
  mem.Store(10, 15);

  // A = 1
  cpu::uint_t inst = oneOperand(cpu::OpCode::LoadA, 14);
  mem.Store(inst, 0);
  // B = 10
  inst = oneOperand(cpu::OpCode::LoadB, 15);
  mem.Store(inst, 1);
  // jump addr:5
  inst = oneOperand(cpu::OpCode::Jump, 5);
  mem.Store(inst, 2);
  // Halt (skipped)
  inst = oneOperand(cpu::OpCode::Halt, 0);
  mem.Store(inst, 3);

  // addr 5: A-B=-9
  inst = twoOperand(cpu::OpCode::Sub, 0x01, 0x02);
  mem.Store(inst, 5);

  cpu->Run();

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == -9);
}

void testJumpNegative() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();

  mem.Store(-3, 14);
  mem.Store(1, 15);

  // A = -3
  cpu::uint_t inst = oneOperand(cpu::OpCode::LoadA, 14);
  mem.Store(inst, 0);
  // B = 1
  inst = oneOperand(cpu::OpCode::LoadB, 15);
  mem.Store(inst, 1);
  // A = B+A
  inst = twoOperand(cpu::OpCode::Add, 0x02, 0x01);
  mem.Store(inst, 2);
  // Jump prev if A < 0
  inst = oneOperand(cpu::OpCode::JumpNeg, 2);
  mem.Store(inst, 3);

  cpu->Run();

  auto result = static_cast<cpu::int_t>(cpu->GetRegisterA());
  assert(result == 0);
}

void testMove() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();

  mem.Store(1, 14);
  mem.Store(99, 15);

  // A = 1;
  cpu::uint_t inst = oneOperand(cpu::OpCode::LoadA, 14);
  mem.Store(inst, 0);
  // B = 99;
  inst = oneOperand(cpu::OpCode::LoadB, 15);
  mem.Store(inst, 1);
  // A = B;
  inst = twoOperand(cpu::OpCode::Mov, 0x02, 0x01);
  mem.Store(inst, 2);

  assert(cpu->GetRegisterA() == 0);
  cpu->Run();
  assert(cpu->GetRegisterA() == 99);
}

void testStore() {
  auto* cpu = new cpu::CPU();
  Memory& mem = cpu->GetMemory();

  mem.Store(26, 15);
  // A = 26;
  cpu::uint_t inst = oneOperand(cpu::OpCode::LoadA, 15);
  mem.Store(inst, 0);
  // mem[14] = A;
  inst = oneOperand(cpu::OpCode::StoreA, 14);
  mem.Store(inst, 1);

  assert(cpu->GetMemory().Load(14) == 0);
  cpu->Run();
  assert(cpu->GetMemory().Load(14) == 26);
}

void RunAllTests() {
  testAddNumbers();
  testJump();
  testJumpNegative();
  testMove();
  testStore();
}