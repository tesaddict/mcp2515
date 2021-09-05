#include <avr/io.h>
#include <string.h>
#include <assert.h>
#include "mcp2515.h"
#include "print.h"

int main(void) {
    usart_init();
    usart_send("Initializing MCP2515.\r\n");
    mcp2515_init();
}
