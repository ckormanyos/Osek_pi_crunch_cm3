Osek_pi_crunch_cm3
==================

<p align="center">
    <a href="https://github.com/ckormanyos/Osek_pi_crunch_cm3/actions">
        <img src="https://github.com/ckormanyos/Osek_pi_crunch_cm3/actions/workflows/Osek_pi_crunch_cm3.yml/badge.svg" alt="Build Status"></a>
</p>

This repository uses an OSEK-like OS on bare-metal ARM(R) Cortex(R)-M3 to calculate $\pi$
with a spigot algorithm.

Osek_pi_crunch_cm3 is a fascinating, educational and fun project
that computes a up to $100,001$ decimal digits of $\pi$
on a bare-metal ARM(R) Cortex(R)-M3 system.

The backbone real-time operating system is taken directly
from the OSEK-like OS implemented in
[Chalandi/OSEK](https://github.com/Chalandi/OSEK)

# Software Details

For the $\pi$-algorithm, we use a (slow) quadratic pi-spigot algorithm
of order $N^2$ in this project. The spigot calculation
is slower than other well-known algorithms (such as AGM).

The calculation memory grows linearly with the digit count.
It requires about 1.4 Mbyte RAM for the full $10^{5}$ decimal-digit
calculation. Since this is significantly more RAM than is
available on-chip, a slow external serial SPI SRAM is used.

GNU/GCC `gcc-arm-non-eabi` is used for target system
development on `*nix`. The build system is based on
Standard GNUmake/shell-script.

# Prototype Project

This repo features a fully-worked-out prototype example project.
The prototype runs on an ARM(R) Cortex(R)-M3 controller fitted on the
SMT32F100RB-NUCLEO board. The board is driven in OS-less, bare-metal mode.

Upon successful calculation completion, the blinky
application toggles pin PB9.
This provides a measure of success viewable with a digital oscilloscope.

Perpetual blinky at 1/2 Hz on the two user-LEDs indicate system-OK and
numerical correctness of the current, finished spigot calculation.
This provides a clear, visual success-indication.

Bit banging is used to implement an all-software SPI-compatible
driver which controls the external SRAM memory chip.

The output pin connections from the board to the SRAM chip
are shown in the table below.

| NUCLEO PIN    | SRAM PIN  | SPI Function               |
| ------------- | --------- | -------------------------- |
| PA11          | 1         | CE (chip-select-not)       |
| PA10          | 2         | SO (chip-serial-out)       |
| PA09          | 6         | CLK (chip-serial-clock)    |
| PA08          | 5         | SI (chip-serial-in)        |

The hardware setup is pictured in the image below.

![](./images/Osek_pi_crunch_cm3.jpg)

# _Bonus_ Generic Serial SPI SRAM driver

The serial SRAM driver is a nice _by_-_product_ of this project.

It has been written in C++14 utilizing a fully generic,
multi-chip, variable-page-size, template form.
This driver can be found in the file
[mcal_memory_sram_generic_spi.h](./Application/ref_app/src/mcal_memory/mcal_memory_sram_generic_spi.h).

Using this SRAM driver requires providing an independent
SPI driver having particular interface functions such as `send()`,
`send_n()` and `recv()`.
