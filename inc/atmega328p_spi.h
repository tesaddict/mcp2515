#ifndef _ATMEGA328P_SPI_H_
#define _ATMEGA328P_SPI_H_
#include <avr/io.h>

// Useful macros
#define UNSET_BIT(A, B) ((A) &= ~(1 << (B)))
#define SET_BIT(A, B) ((A) |=  (1 << (B)))
#define SS_LOW UNSET_BIT(PORTB, PINB2)
#define SS_HIGH SET_BIT(PORTB, PINB2)

void atmega328p_spi_init(void);
void atmega328p_spi_write(const uint8_t *data, const uint8_t size);
void atmega328p_spi_read(uint8_t *data, const uint8_t size);

#endif
