#include <avr/io.h>
#include <avr/interrupt.h>

#include "mcp2515.h"
#include "logger.h"

#define MYUBRR F_CPU/16/BAUD-1

void usart_init(void);
void usart_send(const char *data);

ISR (PCINT0_vect) {
    usart_send("ERROR\r\n");
}

void enable_interrupts(void) {
    SET_BIT(DDRB,  PINB0);
    SET_BIT(PORTB, PINB0);
    SREG |= (1 << 7);
    PCICR &= 0;
    PCICR  |= (1 << PCIE0);
    PCMSK0 &= 0;
    PCMSK0 |= (1 << PCINT0);
}

int main(void) {
    logger_t *logger = logger_alloc(&usart_init, &usart_send, DEBUG);
    enable_interrupts();
    logger_log(logger, INFO, "Initializing MCP2515.\r\n");
    mcp2515_config_t config;
    mcp2515_config_init(&config, KBPS_125, LOOPBACK);
    config.ie.MER_RE = true;
    mcp2515_init(&config);
    logger_log(logger, INFO, "Initialization complete.\r\n"); 

    mcp2515_frame_t can_frame;
    can_frame.id = 42;
    can_frame.data_sz = 5;
    can_frame.data[0] = 'H';
    can_frame.data[1] = 'e';
    can_frame.data[2] = 'l';
    can_frame.data[3] = 'l';
    can_frame.data[4] = 'o';
    can_frame.frame = STANDARD;
    logger_log(logger, INFO, "Sending Standard CAN frame message.\r\n");
    mcp2515_send(&can_frame);
    logger_log(logger, INFO, "Standard CAN frame message has been sent.\r\n");
    mcp2515_frame_t recv_frame;
    mcp2515_recv(&recv_frame);
    logger_log(logger, INFO, "Received CAN frame message.\r\n");
    recv_frame.data[5] = '\r';
    recv_frame.data[6] = '\n';
    recv_frame.data[7] = '\0';
    logger_log(logger, INFO, (char*)recv_frame.data);
    while(1);
    return 0;
}

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
