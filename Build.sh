#!/bin/bash

# ///////////////////////////////////////////////////////////////////
# //                                                               //
# //  Copyright Christopher Kormanyos 2024.                        //
# //  Distributed under the Boost Software License,                //
# //  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
# //  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
# //                                                               //
# ///////////////////////////////////////////////////////////////////

if [[ "$1" != "" ]]; then
    PI_SPIGOT_DIGITS="$1"
else
    PI_SPIGOT_DIGITS=10000
fi

make all PI_SPIGOT_DIGITS=$PI_SPIGOT_DIGITS
