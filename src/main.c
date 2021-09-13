#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h>

#include "mcp2515.h"
#include "logger.h"

#define MYUBRR F_CPU/16/BAUD-1

void usart_init(void);
void usart_send(const char *data);

ISR (PCINT0_vect) {
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
    logger_log(logger, INFO, "TEST: Initializing MCP2515.\r\n");
    mcp2515_config_t config;
    mcp2515_config_init(&config, KBPS_125, LOOPBACK);
    mcp2515_init(&config);
    logger_log(logger, INFO, "PASSED: Initialization complete.\r\n"); 
    
    mcp2515_frame_t send_frame;
    send_frame.id = 42;
    send_frame.data_sz = 5;
    send_frame.data[0] = 'H';
    send_frame.data[1] = 'e';
    send_frame.data[2] = 'l';
    send_frame.data[3] = 'l';
    send_frame.data[4] = 'o';
    send_frame.data[5] = '\0';
    send_frame.frame = STANDARD;
    logger_log(logger, INFO, "TEST: Sending Standard CAN frame message.\r\n");
    mcp2515_send(&send_frame);
    logger_log(logger, INFO, "PASSED: Standard CAN frame message has been sent.\r\n");
    mcp2515_frame_t recv_frame;
    logger_log(logger, INFO, "TEST: Receiving CAN frame message.\r\n");
    int8_t ret = mcp2515_recv(&recv_frame);
    if (ret == 0) {
        logger_log(logger, INFO, "PASSED: Received CAN frame message.\r\n");
    } else {
        logger_log(logger, INFO, "FAILED: No CAN frame message has been received.\r\n");
        return -1;
    }
    recv_frame.data[5] = '\0';
    logger_log(logger, INFO, "TEST: Checking if sent data matches received data.\r\n");
    if (strcmp((char*)send_frame.data, (char*)recv_frame.data) == 0) {
        logger_log(logger, INFO, "PASSED: Sent data matches received data.\r\n");
    } else {
        logger_log(logger, INFO, "FAILED: Sent data does not match received data.\r\n");
        return -1;
    }
    logger_log(logger, INFO, "TEST: Checking if sent id matches received id.\r\n");
    if (recv_frame.id == send_frame.id) {
        logger_log(logger, INFO, "PASSED: Sent id matches received id.\r\n");
    } else {
        logger_log(logger, INFO, "FAILED: Sent id does not match received id.\r\n");
    }
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
