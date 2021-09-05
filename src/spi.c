#include "spi.h"

void spi_master_init(void) {
	SPI_DDR = (1 << SS) | (1 << MOSI) | (1 << SCK);
    SPCR    = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (1 << SPR0);
}

uint8_t spi_master_put(uint8_t data) {
    SPDR = data;
    while(!(SPSR & (1 << SPIF)));
    return SPDR;
}
