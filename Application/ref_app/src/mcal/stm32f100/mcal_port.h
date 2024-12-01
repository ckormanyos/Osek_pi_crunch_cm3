///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_PORT_2012_06_27_H
  #define MCAL_PORT_2012_06_27_H

  #include <mcal_reg.h>

  namespace mcal
  {
    namespace port
    {
      using config_type = void;
      auto init(const config_type*) -> void;

      template<typename addr_type,
               typename reg_type,
               const addr_type port,
               const reg_type bpos>
      class port_pin
      {
      public:
        static void set_direction_output()
        {
          // Read the value of the port direction register.
          // Clear all the port pin control bits in the new register value.
          // Set the port pin control bits for output, push-pull, 50MHz in the new register value.
          // Set the port for digital output.

          using local_reg_access_static_type =
            mcal::reg::reg_access_static<addr_type,
                                         reg_type,
                                         pdir,
                                         static_cast<reg_type>(static_cast<reg_type>(UINT8_C(0x3)) << pdir_shift)>;

          local_reg_access_static_type::template reg_msk<static_cast<reg_type>(static_cast<reg_type>(UINT8_C(0xF)) << pdir_shift)>();
        }

        static void set_direction_input()
        {
          // Read the value of the port direction register.
          // Clear all the port pin control bits in the new register value.
          // Set the port pin control bits for input in the new register value.
          // Set the port for digital input.
          using local_reg_access_static_type =
            mcal::reg::reg_access_static<addr_type,
                                         reg_type,
                                         pdir,
                                         static_cast<reg_type>(static_cast<reg_type>(UINT8_C(0x4)) << pdir_shift)>;

          local_reg_access_static_type::template reg_msk<static_cast<reg_type>(static_cast<reg_type>(UINT8_C(0xF)) << pdir_shift)>();
        }

        static void set_pin_high()
        {
          // Set the port output value to high.
          mcal::reg::reg_access_static<addr_type, reg_type, port, bpos>::bit_set();
        }

        static void set_pin_low()
        {
          // Set the port output value to low.
          mcal::reg::reg_access_static<addr_type, reg_type, port, bpos>::bit_clr();
        }

        static bool read_input_value()
        {
          // Read the port input value.
          return mcal::reg::reg_access_static<addr_type, reg_type, pinp, bpos>::bit_get();
        }

        static void toggle_pin()
        {
          // Toggle the port output value.
          mcal::reg::reg_access_static<addr_type, reg_type, port, bpos>::bit_not();
        }

      private:
        static constexpr addr_type pdir = static_cast<addr_type>(port - static_cast<addr_type>((bpos >= static_cast<reg_type>(UINT8_C(8))) ? static_cast<addr_type>(UINT8_C(8)) : static_cast<addr_type>(UINT8_C(12))));
        static constexpr addr_type pinp = static_cast<addr_type>(port - static_cast<addr_type>(UINT8_C(4)));

        static constexpr reg_type pdir_shift = static_cast<reg_type>((bpos - static_cast<reg_type>((bpos >= static_cast<reg_type>(UINT8_C(8))) ? static_cast<reg_type>(UINT8_C(8)) : static_cast<reg_type>(UINT8_C(0)))) * static_cast<reg_type>(UINT8_C(4)));
      };
    }
  }

#endif // MCAL_PORT_2012_06_27_H
