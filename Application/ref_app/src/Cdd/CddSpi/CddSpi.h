///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2023 - 2024.                        //
//  Copyright Christopher Kormanyos 2024.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#ifndef CDD_SPI_2024_12_03_H
#define CDD_SPI_2024_12_03_H

#include <stdint.h>

#if defined (__cplusplus)
extern "C"
{
#endif

uint8_t CddSpi_TransferSingleByte(const uint8_t byte_to_transfer);
void CddSpi_CsEnable();
void CddSpi_CsDisable();

#if defined (__cplusplus)
}
#endif

#endif // CDD_SPI_2024_12_03_H
