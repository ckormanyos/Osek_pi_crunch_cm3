#!/bin/bash

if [[ "$1" != "" ]]; then
    PI_SPIGOT_DIGITS="$1"
else
    PI_SPIGOT_DIGITS=10000
fi

make all PI_SPIGOT_DIGITS=$PI_SPIGOT_DIGITS
