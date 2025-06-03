#pragma once
#include <vector>

#include "CPUDefs.h"

class Memory {
public:
  explicit Memory(cpu::uint_t size);

  void Store(cpu::uint_t value, cpu::uint_t addr);
  cpu::uint_t Load(cpu::uint_t addr) const;
  cpu::uint_t Size() const;

private:
  std::vector<cpu::uint_t> m_data;
};
