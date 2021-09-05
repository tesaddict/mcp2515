#ifndef _SPI_H_
#define _SPI_H_

#include <avr/io.h>
#include "definitions.h"

#define SPI_DDR DDRB
#define SS      PINB2
#define MOSI    PINB3
#define MISO    PINB4
#define SCK     PINB5

void    spi_master_init(void);
uint8_t spi_master_put(uint8_t data);
#endif
