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
