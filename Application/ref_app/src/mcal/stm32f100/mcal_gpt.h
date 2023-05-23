///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_GPT_2011_10_20_H_
  #define MCAL_GPT_2011_10_20_H_

  #include <cstdint>

  // Forward declaration of the util::timer template class.
  namespace util
  {
    template<typename unsigned_tick_type>
    class timer;
  }

  namespace mcal
  {
    namespace gpt
    {
      using config_type = void;
      using value_type  = std::uint64_t;

      auto init(const config_type*) -> void;

      class secure final
      {
        static auto get_time_elapsed() -> value_type;

        template<typename unsigned_tick_type>
        friend class util::timer;
      };
    }
  }

#endif // MCAL_GPT_2011_10_20_H_
