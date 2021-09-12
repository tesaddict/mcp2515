#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <assert.h>
#include "mcp2515.h"
#include "print.h"

ISR (PCINT0_vect) {
    usart_send("ERROR\r\n");
}

void enable_interrupts(void) {
    SET_BIT(DDRB,  PINB0);
    SET_BIT(PORTB, PINB0);
    // Enable global interrupts.
    SREG |= (1 << 7);
    PCICR &= 0;
    PCICR  |= (1 << PCIE0);
    PCMSK0 &= 0;
    PCMSK0 |= (1 << PCINT0);
}

int main(void) {
    usart_init();
    enable_interrupts();
    usart_send("Initializing MCP2515.\r\n");
    mcp2515_config_t config;
    mcp2515_config_init(&config, KBPS_125, LOOPBACK);
    config.ie.MER_RE = true;
    mcp2515_init(&config);
    usart_send("Initialization complete.\r\n"); 

    mcp2515_frame_t can_frame;
    can_frame.id = 42;
    can_frame.data_sz = 5;
    can_frame.data[0] = 'H';
    can_frame.data[1] = 'e';
    can_frame.data[2] = 'l';
    can_frame.data[3] = 'l';
    can_frame.data[4] = 'o';
    can_frame.tx = TX0;
    can_frame.frame = STANDARD;
    usart_send("Sending Standard CAN frame message.\r\n");
    mcp2515_send(&can_frame);
    usart_send("Standard CAN frame message has been sent.\r\n");
    mcp2515_frame_t recv_frame;
    mcp2515_recv(&recv_frame);
    usart_send("Received CAN frame message.\r\n");
    recv_frame.data[5] = '\r';
    recv_frame.data[6] = '\n';
    recv_frame.data[7] = '\0';
    usart_send((char*)recv_frame.data);
    while(1) {}
    return 0;
}


