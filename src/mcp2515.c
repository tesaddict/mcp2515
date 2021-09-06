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
    // Enable both receive buffer interrupts, and transmit buffer 0 interrupt.
    mcp2515_write_register(CANINTE, (1 << TX0IE) | (1 << RX1IE) | (1 << RX0IE));
    // Set mcp2515 to normal mode.
    mcp2515_write_register(CANCTRL, 0x0);
    _delay_us(10);
    // Check CANSTAT register to make sure we are in normal mode.
    while((mcp2515_read_register(CANSTAT) >> OPMOD0) != 0);
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

// TODO: Make this function configurable.
int8_t mcp2515_tx0_standard_transmit(uint16_t id, uint8_t *data) {
    mcp2515_write_register(TXB0CTRL, 0x0);
    uint8_t id_h = (id & 0x07ff) >> 3;
    uint8_t id_l = (id & 0x0007);
    // Set standard ID and data length.
    mcp2515_write_register(TXB0SIDH, id_h);
    mcp2515_write_register(TXB0SIDL, (id_l << 5));
    mcp2515_write_register(TXB0DLC,  (1 << DLC3));
    // Set data registers. 
    mcp2515_write_register(TXB0D0, *data);
    mcp2515_write_register(TXB0D1, *(data+1));
    mcp2515_write_register(TXB0D2, *(data+2));
    mcp2515_write_register(TXB0D3, *(data+3));
    mcp2515_write_register(TXB0D4, *(data+4));
    mcp2515_write_register(TXB0D5, *(data+5));
    mcp2515_write_register(TXB0D6, *(data+6));
    mcp2515_write_register(TXB0D7, *(data+7));
    // Initialize transmit.
    mcp2515_write_register(TXB0CTRL, (1 << TXREQ) | (1 << TXP1) | (1 << TXP0));
    // TODO: Create a version of this function that schedules a TXREQ interrupt.
    while(1) {
        uint8_t ctrl_val = mcp2515_read_register(TXB0CTRL);
        if ((ctrl_val & (1 << TXREQ)) == 0) return 0;
        if ((ctrl_val & (1 << TXERR)) > 0) return -1;
        if ((ctrl_val & (1 << ABTF)) > 0)  return -2;
        if ((ctrl_val & (1 << MLOA)) > 0)  return -3;
    }
    return 0;
}
