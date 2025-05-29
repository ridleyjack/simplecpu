#include "Memory.h"

#include "CPUDefs.h"

Memory::Memory(cpu::uint_t size) : m_data(size, 0) {}

void Memory::Store(cpu::uint_t value, cpu::uint_t addr) {
  m_data[addr] = value;
}

cpu::uint_t Memory::Load(cpu::uint_t addr) const {
  return m_data[addr];
}

cpu::uint_t Memory::Size() const {
  return static_cast<int>(m_data.size());
}
