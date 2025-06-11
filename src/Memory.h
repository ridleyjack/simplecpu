#pragma once

#include "Nibble.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class Memory {
public:
  explicit Memory(size_t size);

  void Store(cpu::uint4 value, cpu::uint4 addr);
  cpu::uint4 Load(cpu::uint4 addr) const;
  size_t Size() const;

private:
  std::vector<uint8_t> m_data{};
};
