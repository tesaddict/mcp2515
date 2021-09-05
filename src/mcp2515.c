#include "mcp2515.h"

void mcp2515_reset(void) {
	SS_LOW;
    spi_master_put(CAN_RESET);
	SS_HIGH;
}

void mcp2515_write_register(uint8_t address, uint8_t value) {
	SS_LOW;
    spi_master_put(CAN_WRITE);
    spi_master_put(address);
    spi_master_put(value);
	SS_HIGH;
}

void mcp2515_init(void) {
    spi_master_init();
    mcp2515_reset();
    _delay_us(10);
    /* TODO: Create a function that determines CNF1, CNF2 and CNF3 given a desired bit rate.
     * Assuming F_CPU = 16000000
     * SyncSeg = 1 TQ
     * PropSeg = 1 TQ
     * PS1 Seg = 3 TQ
     * PS2 Seg = 3 TQ
     * Total   = 8 TQ
     * NBT(Nominal Bit Time) @ 125 Kb/s = 8000 ns.
     * TQ = NBT / Total = 1000ns.
     * TQ = 2 * (BRP + 1) / F_CPU
     * BRP = ((TQ * F_CPU) / 2) - 1 = 7
    */
    mcp2515_write_register(CNF1, (1 << SJW0) | (1 << BRP0) | (1 << BRP1) | (1 << BRP2));
	mcp2515_write_register(CNF2, (1 << BTLMODE) | (1 << PHSEG11));
	mcp2515_write_register(CNF3, (1 << PHSEG21));
    // Enable both receive buffer interrupts.
    mcp2515_write_register(CANINTE, (1 << RX1IE) | (1 << RX0IE));
    // Set mcp2515 to normal mode.
    mcp2515_write_register(CANCTRL, 0x0);
}

uint8_t mcp2515_read_register(uint8_t address) {
	SS_LOW;
    spi_master_put(CAN_READ);
    spi_master_put(address);
    uint8_t ret = spi_master_put(0xff);
	SS_HIGH;
    return ret;
}



uint8_t mcp2515_read_status() {
	SS_LOW;
    spi_master_put(CAN_READ_STATUS);
    uint8_t ret = spi_master_put(0xff);
	SS_HIGH;
    return ret;
}

