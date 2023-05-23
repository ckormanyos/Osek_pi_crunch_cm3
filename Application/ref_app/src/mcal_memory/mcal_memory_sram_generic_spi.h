///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_MEMORY_SRAM_GENERIC_SPI_2020_04_11_H
  #define MCAL_MEMORY_SRAM_GENERIC_SPI_2020_04_11_H

  #include <array>

  #include <mcal_cpu.h>
  #include <mcal_memory/mcal_memory_sram_types.h>
  #include <util/utility/util_communication.h>
  #include <util/utility/util_noncopyable.h>

  namespace mcal { namespace memory { namespace sram {

  template<const mcal_sram_uintptr_t ByteSizeTotal,
           const mcal_sram_uintptr_t PageGranularity>
  class mcal_memory_sram_generic_spi : private util::noncopyable
  {
  private:
    // instruction   instruction    HEX       instruction
    //    name          format      code      description
    //
    //    READ        0000 0011     0x03      Read data from memory array beginning at selected address
    //    WRITE       0000 0010     0x02      Write data to memory array beginning at selected address

    static constexpr auto read_cmd  = static_cast<std::uint8_t>(UINT8_C(0x03));
    static constexpr auto write_cmd = static_cast<std::uint8_t>(UINT8_C(0x02));

  public:
    constexpr explicit mcal_memory_sram_generic_spi(util::communication_base& com)
      : my_com(com) { }

    ~mcal_memory_sram_generic_spi() = default;

    auto read(const std::uint32_t address, std::uint8_t* p_byte_to_read) -> bool
    {
      // Read one byte from external serial memory.
      set_channel(address);

      const auto addr_chan =
        static_cast<std::uint32_t>
        (
          address % static_cast<std::uint32_t>(byte_size_total())
        );

      using local_cmd_array_type = std::array<std::uint8_t, static_cast<std::size_t>(UINT8_C(5))>;

      const auto cmd =
        local_cmd_array_type
        {
          read_cmd,
          static_cast<std::uint8_t>(addr_chan >> 16U),
          static_cast<std::uint8_t>(addr_chan >>  8U),
          static_cast<std::uint8_t>(addr_chan >>  0U),
          UINT8_C(0xFF)
        };

      my_com.select();
      my_state_is_ok = (my_com.send_n(cmd.cbegin(), cmd.cend()) && my_state_is_ok);
      my_com.deselect();

      my_state_is_ok = (my_com.recv(*p_byte_to_read) && my_state_is_ok);

      return my_state_is_ok;
    }

    auto read_n(const std::uint32_t address, std::uint8_t* p_data_to_read, const std::size_t count) -> bool
    {
      // Read count bytes from external serial memory.

      if((count == static_cast<std::size_t>(UINT8_C(0))) || (p_data_to_read == nullptr))
      {
        static_cast<void>(address);
        static_cast<void>(p_data_to_read);
        static_cast<void>(count);

        my_state_is_ok = (true && my_state_is_ok);
      }
      else if(count == static_cast<std::size_t>(UINT8_C(1)))
      {
        static_cast<void>(count);

        my_state_is_ok = (read(address, p_data_to_read) && my_state_is_ok);
      }
      else
      {
        if(single_page(address, count))
        {
          set_channel(address);

          const auto addr_chan =
            static_cast<std::uint32_t>
            (
              address % static_cast<std::uint32_t>(byte_size_total())
            );

          using local_cmd_array_type = std::array<std::uint8_t, static_cast<std::size_t>(UINT8_C(4))>;

          const auto cmd =
            local_cmd_array_type
            {
              read_cmd,
              static_cast<std::uint8_t>(addr_chan >> 16U),
              static_cast<std::uint8_t>(addr_chan >>  8U),
              static_cast<std::uint8_t>(addr_chan >>  0U)
            };

          my_com.select();
          my_state_is_ok = (my_com.send_n(cmd.cbegin(), cmd.end()) && my_state_is_ok);

          for(auto i = static_cast<std::size_t>(UINT8_C(0)); i < count; ++i)
          {
            my_state_is_ok = (my_com.send(UINT8_C(0xFF))         && my_state_is_ok);
            my_state_is_ok = (my_com.recv(*(p_data_to_read + i)) && my_state_is_ok);
          }
          my_com.deselect();
        }
        else
        {
          for(auto i = static_cast<std::size_t>(UINT8_C(0)); i < count; ++i)
          {
            const auto addr_i = static_cast<std::uint32_t>(address + static_cast<std::uint32_t>(i));

            my_state_is_ok = (read(addr_i, p_data_to_read + i) && my_state_is_ok);
          }
        }
      }

      return my_state_is_ok;
    }

    auto write(const std::uint32_t address, const std::uint8_t* p_byte_to_write) -> bool
    {
      // Write one byte to external serial memory.

      set_channel(address);

      const auto addr_chan =
        static_cast<std::uint32_t>
        (
          address % static_cast<std::uint32_t>(byte_size_total())
        );

      using local_cmd_array_type = std::array<std::uint8_t, static_cast<std::size_t>(UINT8_C(5))>;

      const auto cmd =
        local_cmd_array_type
        {
          write_cmd,
          static_cast<std::uint8_t>(addr_chan >> 16U),
          static_cast<std::uint8_t>(addr_chan >>  8U),
          static_cast<std::uint8_t>(addr_chan >>  0U),
          *p_byte_to_write
        };

      my_com.select();
      my_state_is_ok = (my_com.send_n(cmd.cbegin(), cmd.cend()) && my_state_is_ok);
      my_com.deselect();

      return my_state_is_ok;
    }

    auto write_n(const std::uint32_t address, const std::uint8_t* p_data_to_write, const std::size_t count) -> bool
    {
      // Write count bytes to external serial memory.

      if((count == static_cast<std::size_t>(UINT8_C(0))) || (p_data_to_write == nullptr))
      {
        static_cast<void>(address);
        static_cast<void>(p_data_to_write);
        static_cast<void>(count);

        my_state_is_ok = (true && my_state_is_ok);
      }
      else if(count == static_cast<std::size_t>(UINT8_C(1)))
      {
        static_cast<void>(count);

        my_state_is_ok = (write(address, p_data_to_write) && my_state_is_ok);
      }
      else
      {
        if(single_page(address, count))
        {
          set_channel(address);

          const auto addr_chan =
            static_cast<std::uint32_t>
            (
              address % static_cast<std::uint32_t>(byte_size_total())
            );

          using local_cmd_array_type = std::array<std::uint8_t, static_cast<std::size_t>(UINT8_C(4))>;

          const auto cmd =
            local_cmd_array_type
            {
              write_cmd,
              static_cast<std::uint8_t>(addr_chan >> 16U),
              static_cast<std::uint8_t>(addr_chan >>  8U),
              static_cast<std::uint8_t>(addr_chan >>  0U)
            };

          my_com.select();
          my_state_is_ok = (my_com.send_n(cmd.cbegin(), cmd.cend()) && my_state_is_ok);

          for(auto i = static_cast<std::size_t>(UINT8_C(0)); i < count; ++i)
          {
            my_state_is_ok = (my_com.send(*(p_data_to_write + i)) && my_state_is_ok);
          }
          my_com.deselect();
        }
        else
        {
          for(auto i = static_cast<std::size_t>(UINT8_C(0)); i < count; ++i)
          {
            const auto addr_i = static_cast<std::uint32_t>(address + static_cast<std::uint32_t>(i));

            my_state_is_ok = (write(addr_i, p_data_to_write + i) && my_state_is_ok);
          }
        }
      }

      return my_state_is_ok;
    }

    constexpr auto state_is_ok() const noexcept -> bool { return my_state_is_ok; }

  private:
    util::communication_base& my_com;
    bool my_state_is_ok { true };

    mcal_memory_sram_generic_spi() = delete;

    static constexpr auto byte_size_total() noexcept -> mcal_sram_uintptr_t { return static_cast<mcal_sram_uintptr_t>(ByteSizeTotal); }

    static constexpr auto page_granularity() -> mcal_sram_uintptr_t { return static_cast<mcal_sram_uintptr_t>(PageGranularity); }

    auto set_channel(const std::uint32_t address) -> void
    {
      const auto channel_index =
        static_cast<std::size_t>
        (
          address / static_cast<std::uint32_t>(byte_size_total())
        );

      static_cast<void>(my_com.select_channel(channel_index));
    }

    static constexpr auto single_page(std::uint32_t address, std::size_t count) noexcept -> bool
    {
      const auto current_page = static_cast<std::uint32_t>(address / static_cast<std::uint32_t>(page_granularity()));

      const auto last_page =
        static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>
            (
              address + static_cast<std::uint32_t>(count - static_cast<std::size_t>(UINT8_C(1)))
            )
          / static_cast<std::uint32_t>(page_granularity())
        );

      return (last_page == current_page);
    }
  };

  } } } // namespace mcal::memory::sram

#endif // MCAL_MEMORY_SRAM_GENERIC_SPI_2020_04_11_H
