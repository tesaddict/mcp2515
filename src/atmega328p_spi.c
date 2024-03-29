#include "atmega328p_spi.h"

void atmega328p_spi_init(void) {
  SS_HIGH;
	DDRB = (1 << PINB2) | (1 << PINB3) | (1 << PINB5);
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void atmega328p_spi_transact(uint8_t *buffer, const uint8_t size) {
  SS_LOW;
  for (uint8_t idx = 0; idx < size; ++idx) {
    SPDR = buffer[idx];
    while(!(SPSR & (1 << SPIF)));
    buffer[idx] = SPDR;
  }
  SS_HIGH;
}