#pragma once

#include <cstdint>
#include <iosfwd>
#include <iostream>

namespace cpu {

struct int4;
struct uint4;

// uint4 simulates a 4 bit unsigned number. The underlying data type is a byte.
struct uint4 {
  // Constructors.
  uint4() = default;

  explicit constexpr uint4(const int4& rhs);

  explicit constexpr uint4(const uint8_t v)
      : m_value{static_cast<uint8_t>(m_mask4(v))} {}

  explicit constexpr uint4(const int v)
      : m_value{static_cast<uint8_t>(m_mask4(v))} {}

  // Assignment, Comparison
  constexpr bool operator==(const uint4 v) const noexcept {
    return m_value == v;
  }

  // Assignment, Comparison (uint8_t).
  constexpr uint4& operator=(const uint8_t v) noexcept {
    m_value = m_mask4(v);
    return *this;
  }
  constexpr bool operator==(const uint8_t v) const noexcept {
    return m_value == v;
  }

  // Non-mutating operations.
  constexpr uint4 operator+(const uint4 n) const noexcept {
    return uint4{m_mask4(m_value + n.Raw())};
  }
  constexpr uint4 operator-(const uint4 n) const noexcept {
    return uint4{m_mask4(m_value - n.Raw())};
  }
  constexpr uint4 operator*(const uint4 n) const noexcept {
    return uint4{m_mask4(m_value * n.Raw())};
  }
  constexpr uint4 operator/(const uint4 n) const noexcept {
    return uint4{static_cast<uint8_t>(m_value / n.Raw())};
  }
  constexpr uint4 operator%(const uint4 n) const noexcept {
    return uint4{static_cast<uint8_t>(m_value % n.Raw())};
  }

  // Non-mutating operations (uint8_t).
  constexpr uint4 operator*(const uint8_t n) const noexcept {
    return uint4{m_mask4(m_value * n)};
  }
  constexpr uint4 operator/(const uint8_t n) const noexcept {
    return uint4{static_cast<uint8_t>(m_value / n)};
  }
  constexpr uint4 operator%(const uint8_t n) const noexcept {
    return uint4{static_cast<uint8_t>(m_value % n)};
  }

  constexpr bool operator<(const uint8_t n) const noexcept {
    return m_value < n;
  }
  constexpr bool operator>(const uint8_t n) const noexcept {
    return m_value > n;
  }

  // Non-mutating bit wise operations.
  constexpr uint4 operator&(const uint4& v) const noexcept {
    return uint4{static_cast<uint8_t>(m_value & v.Raw())};
  }
  constexpr uint4 operator|(const uint4& v) const noexcept {
    return uint4{static_cast<uint8_t>(m_value | v.Raw())};
  }

  // Non-mutating bit-wise operations (uint8_t).
  constexpr uint4 operator>>(const uint8_t n) const noexcept {
    return uint4{static_cast<uint8_t>(m_value >> n)};
  }
  constexpr uint4 operator<<(const uint8_t& n) const noexcept {
    return uint4{m_mask4(static_cast<uint8_t>(m_value << n))};
  }
  constexpr uint4 operator&(const uint8_t& v) const noexcept {
    return uint4{static_cast<uint8_t>(m_value & v)};
  }
  constexpr uint4 operator|(const uint8_t& v) const noexcept {
    return uint4{m_mask4(static_cast<uint8_t>(m_value | v))};
  }

  // Mutating operations.
  constexpr uint4 operator++() noexcept {
    m_value = m_mask4(m_value + 1);
    return *this;
  }
  constexpr uint4 operator++(int) noexcept {
    const uint4 temp{*this};
    ++(*this);
    return temp;
  }

  constexpr uint4 operator--() noexcept {
    m_value = m_mask4(m_value - 1);
    return *this;
  }
  constexpr uint4 operator--(int) noexcept {
    const uint4 temp{*this};
    --(*this);
    return temp;
  }

  // Mutating bit-wise operations.
  constexpr uint4& operator&=(const uint8_t v) noexcept {
    m_value &= v;
    return *this;
  }
  constexpr uint4& operator|=(const uint8_t& v) noexcept {
    m_value = m_mask4(m_value | v);
    return *this;
  }
  constexpr uint4& operator~() noexcept {
    m_value = m_mask4(~m_value);
    return *this;
  }

  // Type Conversion.
  explicit constexpr operator uint8_t() const noexcept {
    return m_value;
  }

  constexpr uint8_t Raw() const noexcept {
    return m_value;
  }

private:
  uint8_t m_value{};

  static constexpr uint8_t m_mask4(const uint8_t v) noexcept {
    return v & 0xF;
  }
};

// int4 simulates a 4 bit signed number. The underlying data type is a byte.
struct int4 {
  // Constructors.
  int4() = default;

  explicit constexpr int4(const int8_t v) : m_value{m_mask4(v)} {}

  explicit constexpr int4(const uint4& v);

  // Comparison.
  constexpr bool operator==(const int4& rhs) const noexcept {
    return m_value == rhs.m_value;
  }

  // Assignment, Comparison (int8_t).
  constexpr int4& operator=(const int8_t& other) noexcept {
    m_value = static_cast<int8_t>(other & 0xF);
    return *this;
  }
  constexpr bool operator==(const int8_t v) const noexcept {
    return m_value == v;
  }

  // Type Conversion.
  explicit constexpr operator int8_t() const noexcept {
    if (m_value & 0x08) {
      return static_cast<int8_t>(m_value | 0xF0);
    }
    return m_value;
  }

  constexpr int8_t Raw() const noexcept {
    return m_value;
  }

private:
  int8_t m_value{};

  static constexpr int8_t m_mask4(const int8_t v) noexcept {
    return static_cast<int8_t>(v & 0xF);
  }
};

// uint4
constexpr uint4::uint4(const int4& rhs)
    : m_value{static_cast<uint8_t>(rhs.Raw())} {}

inline std::ostream& operator<<(std::ostream& os, const uint4& v) {
  return os << static_cast<int16_t>(v.Raw());
}

// int4
constexpr int4::int4(const uint4& v) : m_value{static_cast<int8_t>(v.Raw())} {}

inline std::ostream& operator<<(std::ostream& os, const int4& v) {
  return os << static_cast<int16_t>(static_cast<int8_t>(v));
}

} // namespace cpu
