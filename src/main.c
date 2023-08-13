#include <avr/io.h>
#include <avr/interrupt.h>

#include "mcp2515.h"
#include "atmega328p_spi.h"
#include "logger.h"

#define MYUBRR F_CPU/16/BAUD-1

#define TEST_ID 42U
#define TEST_ARRAY_SZ 8U
static const uint8_t TEST_ARRAY_OUT[TEST_ARRAY_SZ] = "12345678";
static uint8_t TEST_ARRAY_IN[TEST_ARRAY_SZ] = { 0 };

void usart_init(void);
void usart_send(const char *data);

int main(void) {
  logger_init(&usart_init, &usart_send, DEBUG);
  atmega328p_spi_init();
  mcp2515_init(KBPS_125, LOOPBACK, &atmega328p_spi_write, &atmega328p_spi_read);
  logger(INFO, "PASSED: Initialization complete.\r\n");
  mcp2515_set_id(TEST_ID);
  mcp2515_send(TEST_ARRAY_OUT, TEST_ARRAY_SZ);
  logger(INFO, "PASSED: Standard CAN frame message has been sent.\r\n");
  uint8_t size = 0;
  int32_t id = mcp2515_recv(TEST_ARRAY_IN, &size);
  if (id != -1) {
      logger(INFO, "PASSED: Received CAN frame message.\r\n");
  } else {
      logger(INFO, "FAILED: No CAN frame message has been received.\r\n");
  }
  if (TEST_ARRAY_IN[0] == TEST_ARRAY_OUT[0]) {
    logger(INFO, "PASSED: Sent data matches received data.\r\n");
  } else {
    logger(INFO, "FAILED: Sent data does not match received data.\r\n");
  }
  if (id == 42) {
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
