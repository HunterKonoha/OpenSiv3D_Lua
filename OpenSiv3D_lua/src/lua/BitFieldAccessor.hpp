#pragma once
#include <cstddef>
#include <limits>

namespace s3d::Lua::BitFieldAccessor {
  template<std::size_t Size, typename enabler = void>
  struct BitFieldType {
    using type = uint64;
  };

  template<std::size_t Size>
  struct BitFieldType<Size, std::enable_if_t<(Size <= 1)>> {
    using type = bool;
  };

  template <std::size_t Size>
  struct BitFieldType<Size, std::enable_if_t<(Size > 2 && Size <= 16)>> {
    using type = uint16;
  };

  template <std::size_t Size>
  struct BitFieldType<Size, std::enable_if_t<(Size > 16 && Size <= 32)>> {
    using type = uint32;
  };

  template <std::size_t Size>
  struct BitFieldType<Size, std::enable_if_t<(Size > 32 && Size <= 64)>> {
    using type = uint64;
  };

  template<std::size_t Size>
  using BitFieldType_t = typename BitFieldType<Size>::type;
  template <typename Base, std::size_t bit_target = 0x0, std::size_t size = 0x1>
  void write(Base& b, BitFieldType_t<size> bits) {
    using aligned_type = BitFieldType_t<sizeof(Base) * CHAR_BIT>;
    static const std::size_t aligned_type_bit_size = sizeof(aligned_type) * CHAR_BIT;
    static_assert(sizeof(Base) * CHAR_BIT >= (bit_target + size), "bit offset and size are too large for the desired structure.");
    static_assert((bit_target % aligned_type_bit_size) <= ((bit_target + size - 1) % aligned_type_bit_size), "bit offset and size cross beyond largest integral constant boundary.");
    
    const std::size_t aligned_target = (bit_target + size - 1) / aligned_type_bit_size;
    const aligned_type bits_left = static_cast<aligned_type>(bit_target - aligned_target);
    const aligned_type shifted_mask = ((static_cast<aligned_type>(1) << size) - 1) << bits_left;
    const aligned_type compl_shifted_mask = ~shifted_mask;
    // Jump by native size of a pointer to target
    // then OR the bits
    aligned_type* jumper = static_cast<aligned_type*>(static_cast<void*>(&b));
    jumper += aligned_target;
    aligned_type& aligned = *jumper;
    aligned &= compl_shifted_mask;
    aligned |= (static_cast<aligned_type>(bits) << bits_left);
  }

  template <typename Base, std::size_t bit_target = 0x0, std::size_t size = 0x1>
  BitFieldType_t<size> read(Base& b) {
    using aligned_type = BitFieldType_t<sizeof(Base) * CHAR_BIT>;
    using field_type = BitFieldType_t<size>;
    static const std::size_t aligned_type_bit_size = sizeof(aligned_type) * CHAR_BIT;
    static_assert(sizeof(Base) * CHAR_BIT >= (bit_target + size), "bit offset and size are too large for the desired structure.");
    static_assert((bit_target % aligned_type_bit_size) <= ((bit_target + size - 1) % aligned_type_bit_size), "bit offset and size cross beyond largest integral constant boundary.");

    const std::size_t aligned_target = (bit_target + size - 1) / aligned_type_bit_size;
    const aligned_type bits_left = static_cast<aligned_type>(bit_target - aligned_target);
    const aligned_type mask = (static_cast<aligned_type>(1) << size) - 1;
    // Jump by native size of a pointer to target
    // then OR the bits
    aligned_type* jumper = static_cast<aligned_type*>(static_cast<void*>(&b));
    jumper += aligned_target;
    const aligned_type& aligned = *jumper;
    aligned_type field_bits = (aligned >> bits_left) & mask;
    field_type bits;
    if constexpr(std::is_same_v<bool, field_type>) {
      bits = field_bits != 0;
    }
    else {
      bits = static_cast<field_type>(field_bits);
    }
    return bits;
  }
}