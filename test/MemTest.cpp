#include "CPUDefs.h"

#include <array>
#include <cassert>
#include <sys/types.h>

#include "Memory.h"

void testSimple() {
  Memory memory{cpu::MemSizeInWords};

  // Stored in high bits.
  cpu::uint4 testInt{7};
  memory.Store(testInt, cpu::uint4(0));
  assert(memory.Load(cpu::uint4(0)) == testInt);

  // Stored in low bits.
  testInt = 4;
  memory.Store(testInt, cpu::uint4(0));
  assert(memory.Load(cpu::uint4(0)) == testInt);
}

void testMultiple() {
  Memory memory{cpu::MemSizeInWords};
  auto testSeries = [&memory](const std::array<uint8_t, 4>& runInt8) {
    std::array<cpu::uint4, 4> run{};
    for (size_t i = 0; i < runInt8.size(); i++) {
      run[i] = static_cast<uint8_t>(runInt8[i]);
    }

    for (int i = 0; i < run.size(); i++) {
      memory.Store(run[i], cpu::uint4(i));
    }
    for (int i = 0; i < run.size(); i++) {
      assert(memory.Load(cpu::uint4(i)) == run[i]);
    }
  };

  std::array<uint8_t, 4> testVals{4, 1, 5, 4};
  testSeries(testVals);

  testVals = {5, 3, 10, 15};
  testSeries(testVals);
}

void testFull() {
  Memory memory{cpu::MemSizeInWords};
  std::array<cpu::uint4, cpu::MemSizeInWords> testVals{};

  for (int i = 0; i < testVals.size(); i++) {
    testVals[i] = 15 - i;
    memory.Store(testVals[i], cpu::uint4(i));
  }

  for (int i = 0; i < testVals.size(); i++) {
    assert(memory.Load(cpu::uint4(i)) == testVals[i]);
  }
}

void testSize() {
  Memory memory{16};
  assert(memory.Size() == 16);

  // Memory is rounded up to nearest even number, since 1 byte = 2 nubs;
  memory = Memory(1);
  assert(memory.Size() == 2);
}

void RunAllMemTests() {
  testSimple();
  testMultiple();
  testFull();
  testSize();
}
