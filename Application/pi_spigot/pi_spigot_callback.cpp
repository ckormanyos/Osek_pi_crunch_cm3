#include <Cdd/CddSerLCD/CddSerLCD.h>

#include <util/utility/util_baselexical_cast.h>

#include <cstdint>

auto pi_count_of_calculations() -> std::uint32_t&;

auto pi_count_of_calculations() -> std::uint32_t&
{
  static std::uint32_t my_count { };

  return my_count;
}

auto pi_lcd_progress(const std::uint32_t pi_output_digits10) -> void;

auto pi_lcd_progress(const std::uint32_t pi_output_digits10) -> void
{
  char p_str[static_cast<std::size_t>(UINT8_C(10))] =
  {
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
  };

  const char* p_end = util::baselexical_cast(pi_output_digits10, p_str, p_str + sizeof(p_str));

  CddSerLCD_WriteLine(p_str, static_cast<std::size_t>(p_end - p_str), 0U);

  std::fill(p_str, p_str + sizeof(p_str), (char) 0);

  p_end = util::baselexical_cast(pi_count_of_calculations(), p_str, p_str + sizeof(p_str));

  CddSerLCD_WriteLine(p_str, static_cast<std::size_t>(p_end - p_str), 1U);
}

