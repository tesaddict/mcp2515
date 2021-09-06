#include <avr/io.h>
#include <string.h>
#include <assert.h>
#include "mcp2515.h"
#include "print.h"

void test_transmission(void) {
    uint8_t buf[8];
    for (int i = 0; i < 8; ++i) {
        buf[i] = i;
    }
    usart_send("Sending data buffer with standard CAN frame.\r\n");
    int ret = mcp2515_tx0_standard_transmit(10, buf);
    if (ret == -3) {
        usart_send("Sending data has failed. Arbitration error detected.\r\n");
    } else if (ret == -2) {
        usart_send("Sending data has failed. Message has been aborted.\r\n");
    } else if (ret == -1) {
        usart_send("Sending data has failed. Transmission error detected.\r\n");
    } else {
        usart_send("Sent data successfully.\r\n");
    }
}

int main(void) {
    usart_init();
    usart_send("Initializing MCP2515.\r\n");
    mcp2515_init();
    test_transmission();
    return 0;
}


