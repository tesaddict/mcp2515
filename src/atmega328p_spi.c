#include "atmega328p_spi.h"

void atmega328p_spi_init(void) {
	DDRB = (1 << PINB2) | (1 << PINB3) | (1 << PINB5);
  SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (1 << SPR0);
}

void atmega328p_spi_write(const uint8_t *in, const uint8_t size) {
  for (uint8_t idx = 0; idx < size; ++idx) {
    SPDR = in[idx];
    while(!(SPSR & (1 << SPIF)));
  }
}

void atmega328p_spi_read(uint8_t *out, const uint8_t size) {
  for (uint8_t idx = 0; idx < size; ++idx) {
    SPDR = 0xFFU;
    while(!(SPSR & (1 << SPIF)));
    out[idx] = SPDR;
  }
}

void atmega328p_spi_cs_low(void) {
  SS_LOW;
}

void atmega328p_spi_cs_high(void) {
  SS_HIGH;
}
