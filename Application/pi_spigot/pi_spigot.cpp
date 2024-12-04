﻿///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022.                               //
//  Copyright Christopher Kormanyos 2019 - 2024.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

// pi_spigot.cpp

// This program can be used to compute many thousands
// of decimal digits of digits of pi. It uses a so-called
// "spigot" algorithm having quadratic complexity.

// In this embedded software adaptation of this work,
// we limit the decimal digit count to about 100k or fewer.

// cd /mnt/c/Users/ckorm/Documents/Ks/uC_Software/Boards/Osek_pi_crunch_cm3
// ./Build.sh

// To build pi_spigot.cpp on LINUX host:
// cd /mnt/c/Users/ckorm/Documents/Ks/uC_Software/Boards/Osek_pi_crunch_cm3
// g++ -std=c++20 -Werror -Wall -Wextra -Wpedantic -O3 -DPI_CRUNCH_METAL_STANDALONE_MAIN -DPI_CRUNCH_METAL_PI_SPIGOT_DIGITS=10000 -I./Application/ref_app/src -I./Application ./Application/pi_spigot/pi_spigot.cpp -o pi_spigot.exe

#include <pi_calc_cfg.h>

#include <math/checksums/hash/hash_sha1.h>
#include <math/pi_spigot/pi_spigot.h>
#if !defined(PI_CRUNCH_METAL_STANDALONE_MAIN)
#include <mcal_benchmark.h>
#include <mcal_memory/mcal_memory_sram_array.h>
#endif
#include <util/utility/util_baselexical_cast.h>

#if !defined(PI_CRUNCH_METAL_DISABLE_IOSTREAM)
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#endif // !PI_CRUNCH_METAL_DISABLE_IOSTREAM

namespace local
{
  #if defined(PI_CRUNCH_METAL_PI_SPIGOT_DIGITS)
    #if (PI_CRUNCH_METAL_PI_SPIGOT_DIGITS == PI_CRUNCH_METAL_PI_SPIGOT_USE_100_DIGITS)
    constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(101));
    #elif (PI_CRUNCH_METAL_PI_SPIGOT_DIGITS == PI_CRUNCH_METAL_PI_SPIGOT_USE_1K_DIGITS)
    constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(1001));
    #elif (PI_CRUNCH_METAL_PI_SPIGOT_DIGITS == PI_CRUNCH_METAL_PI_SPIGOT_USE_10K_DIGITS)
    constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(10001));
    #elif (PI_CRUNCH_METAL_PI_SPIGOT_DIGITS == PI_CRUNCH_METAL_PI_SPIGOT_USE_100K_DIGITS)
    constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(100001));
    #endif
  #else
    #error Error: Please define PI_CRUNCH_METAL_PI_SPIGOT_DIGITS
  #endif

  constexpr auto loop_digit = static_cast<std::uint32_t>(UINT8_C(9));

  using pi_spigot_type = math::constants::pi_spigot<result_digit, loop_digit>;

  pi_spigot_type pi_spigot_instance;

  using hash_type = math::checksums::hash::hash_sha1;

  hash_type pi_spigot_hash;

  auto pi_output_digits10 = static_cast<std::uint32_t>(UINT8_C(0));

  #if !defined(PI_CRUNCH_METAL_STANDALONE_MAIN)
  using benchmark_port_type = ::mcal::benchmark::benchmark_port_type;
  #endif

  #if defined(PI_CRUNCH_METAL_STANDALONE_MAIN)
  using mcal_sram_uintptr_t = std::uintptr_t;
  #endif

  constexpr auto pi_spigot_input_start_address = static_cast<mcal_sram_uintptr_t>(UINT8_C(0));

  #if !defined(PI_CRUNCH_METAL_DISABLE_IOSTREAM)
  using pi_spigot_input_container_type = std::array<std::uint32_t,
                                                    pi_spigot_type::input_static_size>;
  #else
  using pi_spigot_input_container_type = mcal::memory::sram::array<std::uint32_t,
                                                                   pi_spigot_type::input_static_size,
                                                                   pi_spigot_input_start_address>;
  #endif

  pi_spigot_input_container_type pi_spigot_input;
} // namespace local

extern "C"
{
  auto mcal_led_toggle() -> void;

  auto pi_main() -> int;

  auto pi_led_toggle() -> void;
}

extern "C"
auto pi_led_toggle() -> void
{
  ::mcal_led_toggle();
}

extern auto pi_lcd_progress(const std::uint32_t pi_output_digits10) -> void;
extern auto pi_count_of_calculations() -> std::uint32_t&;

auto pi_main() -> int
{
  #if !defined(PI_CRUNCH_METAL_STANDALONE_MAIN)
  local::benchmark_port_type::toggle_pin();
  #endif

  local::pi_spigot_instance.calculate(local::pi_spigot_input.data(), pi_lcd_progress, &local::pi_spigot_hash);

  std::uint32_t cnt = pi_count_of_calculations();

  ++cnt;

  pi_count_of_calculations() = cnt;

  // Check the hash result of the pi calculation.
  const auto hash_control =
    typename local::hash_type::result_type
    #if (PI_CRUNCH_METAL_PI_SPIGOT_DIGITS == PI_CRUNCH_METAL_PI_SPIGOT_USE_100_DIGITS)
    {
      0x93U, 0xF1U, 0xB4U, 0xEAU, 0xABU, 0xCBU, 0xC9U, 0xB9U,
      0x0CU, 0x93U, 0x93U, 0x24U, 0xF7U, 0x85U, 0x13U, 0x2EU,
      0xDFU, 0x3BU, 0xF2U, 0x01U
    };
    #elif (PI_CRUNCH_METAL_PI_SPIGOT_DIGITS == PI_CRUNCH_METAL_PI_SPIGOT_USE_1K_DIGITS)
    {
      0xA0U, 0x92U, 0x47U, 0x1FU, 0xD5U, 0xFEU, 0x41U, 0x51U,
      0x20U, 0xE7U, 0xDDU, 0x18U, 0x5BU, 0x93U, 0x0DU, 0x05U,
      0x3AU, 0x86U, 0xF1U, 0x7EU
    };
    #elif (PI_CRUNCH_METAL_PI_SPIGOT_DIGITS == PI_CRUNCH_METAL_PI_SPIGOT_USE_10K_DIGITS)
    {
      // 10001: 4BDF69A5FF25B9BED6BA9802BD2A68306FAB71EC
      0x4BU, 0xDFU, 0x69U, 0xA5U, 0xFFU, 0x25U, 0xB9U, 0xBEU,
      0xD6U, 0xBAU, 0x98U, 0x02U, 0xBDU, 0x2AU, 0x68U, 0x30U,
      0x6FU, 0xABU, 0x71U, 0xECU
    };
    #elif (PI_CRUNCH_METAL_PI_SPIGOT_DIGITS == PI_CRUNCH_METAL_PI_SPIGOT_USE_100K_DIGITS)
    {
      // 100001: D9D56240EB6B626A8FE179E3054D332F1767071D
      0xD9U, 0xD5U, 0x62U, 0x40U, 0xEBU, 0x6BU, 0x62U, 0x6AU,
      0x8FU, 0xE1U, 0x79U, 0xE3U, 0x05U, 0x4DU, 0x33U, 0x2FU,
      0x17U, 0x67U, 0x07U, 0x1DU
    };
    #endif

  using local_hash_type = local::hash_type;
  using local_hash_result_type = typename local_hash_type::result_type;

  local_hash_result_type hash_result { };

  local::pi_spigot_hash.get_result(hash_result.data());

  const bool result_is_ok { std::equal(hash_result.cbegin(), hash_result.cend(), hash_control.cbegin()) };

  const int result_of_pi_main { (result_is_ok ? static_cast<int>(INT8_C(0)) : static_cast<int>(INT8_C(-1))) };

  return result_of_pi_main;
}

#if defined(PI_CRUNCH_METAL_STANDALONE_MAIN)

auto pi_count_of_calculations() -> std::uint32_t&;
auto pi_lcd_progress(const std::uint32_t pi_output_digits10) -> void;

auto pi_count_of_calculations() -> std::uint32_t&
{
  static std::uint32_t my_count { };

  return my_count;
}

auto pi_lcd_progress(const std::uint32_t pi_output_digits10) -> void { static_cast<void>(pi_output_digits10); }

extern "C"
{
  auto mcal_init      () -> void;
  auto mcal_led_toggle() -> void;

  auto mcal_init      () -> void { }
  auto mcal_led_toggle() -> void { }
}

auto main() -> int
{
  ::mcal_init();

  #if !defined(PI_CRUNCH_METAL_DISABLE_IOSTREAM)
  std::stringstream strm { };

  strm << "Begin pi spigot calculation...\n";
  #endif

  const int result_pi_main { ::pi_main() };

  const bool result_is_ok { (result_pi_main == 0) };

  #if !defined(PI_CRUNCH_METAL_DISABLE_IOSTREAM)

  strm << "digits10:     " << local::pi_spigot_type::result_digit() << '\n'
       << "result_is_ok: " << std::boolalpha << result_is_ok;

  std::cout << strm.str() << std::endl;
  #endif

  const int result_of_main { (result_is_ok ? static_cast<int>(INT8_C(0)) : static_cast<int>(INT8_C(-1))) };

  return result_of_main;
}

#else

auto pi_calc_dummy() -> void;

auto pi_calc_dummy() -> void { }

#endif
