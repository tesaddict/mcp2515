#include <avr/io.h>
#include <avr/interrupt.h>

#include "mcp2515.h"
#include "atmega328p_spi.h"
#include "logger.h"

#define MYUBRR F_CPU/16/BAUD-1

void usart_init(void);
void usart_send(const char *data);

int main(void) {
  logger_init(&usart_init, &usart_send, DEBUG);
  atmega328p_spi_init();
  mcp2515_init(KBPS_125, LOOPBACK, &atmega328p_spi_transaction);
  logger(INFO, "PASSED: Initialization complete.\r\n");
  mcp2515_frame_t send_frame;
  send_frame.id = 42;
  send_frame.data_size = 5;
  send_frame.data[0] = 'H';
  send_frame.data[1] = 'e';
  send_frame.data[2] = 'l';
  send_frame.data[3] = 'l';
  send_frame.data[4] = 'o';
  mcp2515_send(&send_frame);
  logger(INFO, "PASSED: Standard CAN frame message has been sent.\r\n");
  mcp2515_frame_t recv_frame;
  int8_t ret = mcp2515_recv(&recv_frame);
  if (ret == 0) {
      logger(INFO, "PASSED: Received CAN frame message.\r\n");
  } else {
      logger(INFO, "FAILED: No CAN frame message has been received.\r\n");
  }
  if (send_frame.data[0] == recv_frame.data[0]) {
    logger(INFO, "PASSED: Sent data matches received data.\r\n");
  } else {
    logger(INFO, "FAILED: Sent data does not match received data.\r\n");
  }
  if (recv_frame.id == send_frame.id) {
    logger(INFO, "PASSED: Sent id matches received id.\r\n");
  } else {
    logger(INFO, "FAILED: Sent id does not match received id.\r\n");
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
