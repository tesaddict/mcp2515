#include "atmega328p_spi.h"

void atmega328p_spi_init(void) {
	DDRB = (1 << PINB2) | (1 << PINB3) | (1 << PINB5);
  SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (1 << SPR0);
}

void atmega328p_spi_write(const uint8_t *buffer, const uint8_t size) {
  SS_LOW;
  for (uint8_t idx = 0; idx < size; ++idx) {
    SPDR = buffer[idx];
    while(!(SPSR & (1 << SPIF)));
  }
  SS_HIGH;
}

void atmega328p_spi_read(uint8_t *buffer, const uint8_t size) {
  SS_LOW;
  for (uint8_t idx = 0; idx < size; ++idx) {
    SPDR = buffer[idx];
    while(!(SPSR & (1 << SPIF)));
    buffer[idx] = SPDR;
  }
  SS_HIGH;
}