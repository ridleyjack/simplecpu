#include "Memory.h"

#include "CPUDefs.h"

Memory::Memory(cpu::uint_t size) : m_data(size / 2 + size % 2, 0) {}

void Memory::Store(cpu::uint_t value, cpu::uint_t addr) {
  size_t idx = addr / 2;
  if (addr % 2 == 1) {
    constexpr cpu::uint_t mask = 0xF0;
    m_data[idx] = m_data[idx] & mask; // Clear the low bits.
    m_data[idx] |= value;             // Value into low bits;
  } else {
    constexpr cpu::uint_t mask = 0x0F;
    m_data[idx] = m_data[idx] & mask; // Clear the high bits.
    m_data[idx] |= value << 4;        // Value into high bits.
  }
}

cpu::uint_t Memory::Load(cpu::uint_t addr) const {
  size_t index = addr / 2;
  if (addr % 2 == 1) {
    constexpr cpu::uint_t mask = 0x0F;
    return m_data[index] & mask; // Grab low bits;
  }
  constexpr cpu::uint_t mask = 0xF0;
  return (m_data[index] & mask) >> 4; // Grab high bits, shift into low.
}

cpu::uint_t Memory::Size() const {
  return static_cast<int>(m_data.size()) * 2;
}
