#ifndef _PRINT_H_
#define _PRINT_H_

#include <avr/io.h>
#include <stdio.h>

#define MYUBRR F_CPU/16/BAUD-1

void usart_init(void);
void usart_send(const char *data);
void byte_to_hex_string(uint8_t byte, char *dest);
#endif
