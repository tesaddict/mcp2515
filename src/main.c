#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "mcp2515.h"
#include "atmega328p_spi.h"
#include "logger.h"

#define MYUBRR F_CPU/16/BAUD-1

#define TEST_FRAME_ID 42U
#define TEST_FRAME_SZ 8U
#define TEST_FRAME_DATA { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 }

#define TEST_MAX_ATTEMPTS 5

static const mcp2515_frame_t TEST_FRAME_OUT = {
  .id = TEST_FRAME_ID,
  .size = TEST_FRAME_SZ,
  .data = TEST_FRAME_DATA,
};

static mcp2515_frame_t TEST_FRAME_IN = { 0 };

void usart_init(void);
void usart_send(const char *data);

static void test_print_frame(const mcp2515_frame_t *frame) {
  logger(INFO, "DATA: ");
  logger_print_bytes_as_hex(frame->data, frame->size);
}

int main(void) {
  logger_init(&usart_init, &usart_send, DEBUG);
  atmega328p_spi_init();
  mcp2515_init(KBPS_125, LOOPBACK, &atmega328p_spi_transact);
  mcp2515_send(&TEST_FRAME_OUT);
  mcp2515_recv(&TEST_FRAME_IN);
  test_print_frame(&TEST_FRAME_OUT);
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
