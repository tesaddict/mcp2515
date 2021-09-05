#include "print.h"

static const char hex[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', 
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

void usart_init(void) {
    const unsigned int ubrr = MYUBRR;
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)(ubrr);
    UCSR0C = 0x06;
    UCSR0B = (1<<TXEN0);
}

void usart_send(const char *data) {
    int i = 0;
    while(data[i] != 0) {
        while (!(UCSR0A & (1<<UDRE0)));
        UDR0 = data[i];
        i++;
    }
}

void byte_to_hex_string(uint8_t byte, char *dest) {
    uint8_t upper = (byte & 0xf0) >> 4; 
    uint8_t lower = (byte & 0x0f);
    *dest       = hex[upper];
    *(dest + 1) = hex[lower];
}

