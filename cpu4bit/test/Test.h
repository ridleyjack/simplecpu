#pragma once

void RunAllMemTests();
void RunAllCPUTests();

inline void RunAllTests() {
  RunAllMemTests();
  RunAllCPUTests();
}