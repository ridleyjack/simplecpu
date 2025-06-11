#include "ALU.h"
#include "CPUDefs.h"

#include <cassert>
#include <cstdint>

namespace cpu {

namespace {
void TestAddPositive() {
  Register result{};
  alu::ALU alu{result};

  // Try all possible combinations of positive numbers.
  for (uint8_t sum = 0; sum <= 7; sum++) {
    for (uint8_t a = 0; a <= sum; a++) {
      const uint8_t b = sum - a;
      const auto inA{static_cast<Register>(a)};
      const auto inB{static_cast<Register>(b)};
      alu.DoOperation(inA, inB, OpCode::Add);
      assert(result == a + b);

      assert(alu.GetFlags().Negative == false);
      assert(alu.GetFlags().Overflow == false);
      assert(alu.GetFlags().Zero == (a + b == 0));
    }
  }
}

void TestAddAll() {
  Register result{};
  alu::ALU alu{result};

  // Try all negative and some positive combinations.
  // Some of these values wrap.
  for (int8_t sum = -8; sum <= 7; sum++) {
    for (int8_t a = -8; a <= sum; a++) {
      int8_t b = sum - a;

      const uint4 inA{static_cast<int4>(a)};
      const uint4 inB{static_cast<int4>(b)};
      alu.DoOperation(inA, inB, OpCode::Add);
      assert(int4(result) == int4(a + b));
    }
  }
}

void TestSubAll() {
  Register result{};
  alu::ALU alu{result};

  for (int8_t sum = -8; sum <= 7; sum++) {
    for (int8_t a = -8; a <= sum; a++) {
      int8_t b = sum - a;

      const uint4 inA{static_cast<int4>(a)};
      const uint4 inB{static_cast<int4>(b)};
      alu.DoOperation(inA, inB, OpCode::Sub);
      assert(int4(result) == int4(a - b));
    }
  }
}

void TestFlags() {
  Register result{};
  alu::ALU alu{result};

  int4 inA{-8};
  int4 inB{-5};
  alu.DoOperation(uint4(inA), uint4(inB), OpCode::Add);
  assert(int4(result) == 3);
  assert(alu.GetFlags().Negative == false);
  assert(alu.GetFlags().Overflow == true);
  assert(alu.GetFlags().Zero == false);

  inA = -4;
  inB = 1;
  alu.DoOperation(uint4(inA), uint4(inB), OpCode::Add);
  assert(int4(result) == int4(-3));
  assert(alu.GetFlags().Negative == true);
  assert(alu.GetFlags().Overflow == false);
  assert(alu.GetFlags().Zero == false);

  inA = 4;
  inB = -4;
  alu.DoOperation(uint4(inA), uint4(inB), OpCode::Add);
  assert(int4(result) == int4(0));
  assert(alu.GetFlags().Negative == false);
  assert(alu.GetFlags().Overflow == false);
  assert(alu.GetFlags().Zero == true);
}

} // namespace

} // namespace cpu

void RunAllALUTests() {
  cpu::TestAddPositive();
  cpu::TestAddAll();
  cpu::TestSubAll();
  cpu::TestFlags();
}
