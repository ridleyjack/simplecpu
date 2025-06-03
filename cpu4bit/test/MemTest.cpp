#include <array>
#include <cassert>
#include <sys/types.h>

#include "Memory.h"

#include <iostream>

void testSimple() {
  Memory memory{16};

  // Stored in high bits.
  uint testInt = 11;
  memory.Store(testInt, 0);
  assert(memory.Load(0) == testInt);

  // Stored in low bits.
  testInt = 4;
  memory.Store(testInt, 1);
  assert(memory.Load(1) == testInt);
}

void testMultiple() {
  Memory memory{16};
  auto testSeries = [&memory](std::array<uint, 4>& run) {
    for (uint i = 0; i < run.size(); i++) {
      memory.Store(run[i], i);
    }
    for (uint i = 0; i < run.size(); i++) {
      assert(memory.Load(i) == run[i]);
    }
  };

  std::array<uint, 4> testVals{4, 1, 5, 4};
  testSeries(testVals);

  testVals = {5, 3, 10, 15};
  testSeries(testVals);
}

void testFull() {
  Memory memory{16};
  std::array<uint, 16> testVals{};

  for (uint i = 0; i < testVals.size(); i++) {
    testVals[i] = 15 - i;
    memory.Store(testVals[i], i);
  }

  for (uint i = 0; i < testVals.size(); i++) {
    assert(memory.Load(i) == testVals[i]);
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
  testSize();
  testFull();
}