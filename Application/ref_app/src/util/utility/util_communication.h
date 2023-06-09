///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef UTIL_COMMUNICATION_2012_05_31_H
  #define UTIL_COMMUNICATION_2012_05_31_H

  #include <algorithm>
  #include <array>
  #include <cstddef>
  #include <cstdint>

  #include <mcal_memory/mcal_memory_sram_types.h>
  #include <util/utility/util_noncopyable.h>

  namespace mcal { namespace spi {

  // Forward declaration of the mcal::spi::spi_software_port_driver class.
  template<typename port_pin_sck__type,
           typename port_pin_mosi_type,
           typename port_pin_csn__type,
           typename port_pin_miso_type,
           const std::uint_fast16_t nop_count,
           const bool has_disable_enable_interrupts>
  class spi_software_port_driver;

  } } // namespace mcal::spi

  namespace mcal { namespace memory { namespace sram {

  template<const mcal_sram_uintptr_t ByteSizeTotal,
           const mcal_sram_uintptr_t PageGranularity>
  class mcal_memory_sram_generic_spi;

  } } } // namespace mcal::memory::sram

  namespace util
  {
    class communication_base : private util::noncopyable
    {
    public:
      using buffer_value_type = std::uint8_t;

      using send_iterator_type = const buffer_value_type*;

      virtual ~communication_base() = default;

      virtual auto recv(std::uint8_t& byte_to_recv) -> bool = 0;

      virtual auto   select() -> void = 0;
      virtual auto deselect() -> void = 0;

      virtual auto send_n(send_iterator_type first, send_iterator_type last) -> bool = 0;

      virtual auto send(const std::uint8_t byte_to_send) -> bool = 0;

    protected:
      communication_base() = default;
    };

    class communication_buffer_depth_one_byte : public communication_base
    {
    private:
      using base_class_type = communication_base;

    public:
      ~communication_buffer_depth_one_byte() override = default;

      communication_buffer_depth_one_byte() = default;

    private:
      base_class_type::buffer_value_type recv_buffer { };

      template<typename port_pin_sck__type,
               typename port_pin_mosi_type,
               typename port_pin_csn__type,
               typename port_pin_miso_type,
               const std::uint_fast16_t nop_count,
               const bool has_disable_enable_interrupts>
      friend class mcal::spi::spi_software_port_driver;

      template<const mcal_sram_uintptr_t ByteSizeTotal,
               const mcal_sram_uintptr_t PageGranularity>
      friend class mcal::memory::sram::mcal_memory_sram_generic_spi;
    };
  }

#endif // UTIL_COMMUNICATION_2012_05_31_H
