#pragma once

void RunAllMemTests();
void RunAllCPUTests();
void RunAllALUTests();

inline void RunAllTests() {
  RunAllALUTests();
  RunAllMemTests();
  RunAllCPUTests();
}