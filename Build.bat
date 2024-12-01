rem
rem  Copyright Christopher Kormanyos 2024.
rem  Distributed under the Boost Software License,
rem  Version 1.0. (See accompanying file LICENSE_1_0.txt
rem  or copy at http://www.boost.org/LICENSE_1_0.txt)
rem

@set build_command=.\make.exe -f ./Makefile %2 PI_SPIGOT_DIGITS=%1

@echo Executing Command: %build_command%

@%build_command%
