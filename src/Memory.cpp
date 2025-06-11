#include "Memory.h"

#include "Nibble.h"

#include <cstdint>
#include <iostream>

// Memory represents (and actually is) volatile memory. It is used to
// simulate the RAM space that the CPU interacts with.
// The memories size must be an even number as 2 uint4s are stored per byte.
// A byte is the smallest size of data that most modern systems can work with.
// If an odd size is specified then the actual capacity will be the next
// even number.
Memory::Memory(const size_t size) : m_data(size / 2 + size % 2, 0) {
  if (size > 16) {
    std::cerr << "Memory is larger than maximum usable size of 16\n";
  }
}

// Two 4int numbers as are stored per byte in the format as follows:
// HHHH LLLL where H=vale stored at lower index. And L= value stored at
// next index.
// Implementation specifics are hidden and the interface allows storing a
// number by simply specifying a value and address.
void Memory::Store(const cpu::uint4 value, const cpu::uint4 addr) {
  const auto idx{static_cast<uint8_t>(addr / 2)};
  if (addr % 2 == 1) { // Odd numbers go into the low bits.
    constexpr uint8_t mask = 0xF0;
    m_data[idx] = m_data[idx] & mask;           // Clear the low bits.
    m_data[idx] |= static_cast<uint8_t>(value); // Value into low bits;
  } else { // Even numbers go into the high bits.
    constexpr uint8_t mask = 0x0F;
    m_data[idx] = m_data[idx] & mask;                // Clear the high bits.
    m_data[idx] |= static_cast<uint8_t>(value) << 4; // Value into high bits.
  }
}

// Load works the opposite of store. All values are initialized to zero so a cpu
// that reads a value it hasn't interacted with will receive a zero.
cpu::uint4 Memory::Load(const cpu::uint4 addr) const {
  const auto idx{static_cast<uint8_t>(addr / 2)};
  if (addr % 2 == 1) { // Odd numbers are found in the low bits.
    constexpr uint8_t mask = 0x0F;
    return cpu::uint4(m_data[idx] & mask); // Grab low bits;
  } //  Even numbers are found in the high bits.
  // Grab high bits, shift into low.
  constexpr uint8_t mask = 0xF0;
  return cpu::uint4((m_data[idx] & mask) >> 4);
}

// Size returns the simulated capacity of the memory space. The size may
// be size+1 of the amount specified in the constructor due to rounding up to
// the nearest even number.
size_t Memory::Size() const {
  return m_data.size() * 2;
}
