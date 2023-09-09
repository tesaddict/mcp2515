#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "mcp2515.h"
#include "atmega328p_spi.h"
#include "logger.h"

#define MYUBRR F_CPU/16/BAUD-1

#define TEST_FRAME_ID 42U
#define TEST_FRAME_SZ 8U
#define TEST_FRAME_DATA { 1, 2, 3, 4, 5, 6, 7, 8 }

#define TEST_MAX_ATTEMPTS 5

static const mcp2515_frame_t TEST_FRAME_OUT = {
  .id = TEST_FRAME_ID,
  .size = TEST_FRAME_SZ,
  .data = TEST_FRAME_DATA,
};

static mcp2515_frame_t TEST_FRAME_IN = { 0 };

void usart_init(void);
void usart_send(const char *data);

static void test_print_status(const char *str, bool cond) {
  if (cond) {
    logger(INFO, "PASSED: ");
  } else {
    logger(INFO, "FAILED: ");
  }
  logger(INFO, str);
  logger(INFO, "\r\n");
}

int main(void) {
  logger_init(&usart_init, &usart_send, DEBUG);
  atmega328p_spi_init();
  mcp2515_init(KBPS_125, LOOPBACK, &atmega328p_spi_transact);
  mcp2515_send(&TEST_FRAME_OUT);

  // There is a delay from sending a frame to receiving it.
  MCP2515_Status_t status;
  for(uint8_t attempts = 0; attempts < TEST_MAX_ATTEMPTS; attempts++) {
    status = mcp2515_recv(&TEST_FRAME_IN);
    if (status == MCP2515_STATUS_SUCCESS) break;
  }

  test_print_status("Receiving CAN frame message", (status == MCP2515_STATUS_SUCCESS));
  test_print_status("Sent ID matches received ID", (TEST_FRAME_IN.id == TEST_FRAME_ID));
  test_print_status("Sent size matches received size", (TEST_FRAME_OUT.size == TEST_FRAME_IN.size));

  bool pass = false;
  for (uint8_t idx = 0; idx < TEST_FRAME_IN.size; idx++) {
    if (TEST_FRAME_OUT.data[idx] != TEST_FRAME_IN.data[idx]) {
      pass = false;
      break;
    }
    pass = true;
  }
  test_print_status("Sent data matches received data", pass);
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
