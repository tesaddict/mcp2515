#include "mcp2515.h"
#include "mcp2515_definitions.h"
#include "definitions.h"

void mcp2515_config_init(
        mcp2515_config_t *config,
        const CAN_SPEED speed,
        const CAN_MODE  mode) 
{
    config->speed = speed;
    config->mode  = mode;
    config->ie.MER_RE = false;
    config->ie.WAK_IE = false;
    config->ie.ERR_IE = false;
    config->ie.TX2_IE = false;
    config->ie.TX1_IE = false;
    config->ie.TX0_IE = false;
    config->ie.RX1_IE = false;
    config->ie.RX0_IE = false;
}

uint8_t mcp2515_get_interrupt_val(const mcp2515_interrupts_t *it) {
    uint8_t ret = 0;
    ret |= it->MER_RE ? (1 << MERRE) : 0;
    ret |= it->WAK_IE ? (1 << WAKIE) : 0;
    ret |= it->ERR_IE ? (1 << ERRIE) : 0;
    ret |= it->TX2_IE ? (1 << TX2IE) : 0;
    ret |= it->TX1_IE ? (1 << TX1IE) : 0;
    ret |= it->TX0_IE ? (1 << TX0IE) : 0;
    ret |= it->RX1_IE ? (1 << RX1IE) : 0;
    ret |= it->RX0_IE ? (1 << RX0IE) : 0;
    return ret;
}

void mcp2515_reset(void) {
	SS_LOW;
    spi_master_put(CAN_RESET);
	SS_HIGH;
}

void mcp2515_write_register(const uint8_t address, const uint8_t value) {
	SS_LOW;
    spi_master_put(CAN_WRITE);
    spi_master_put(address);
    spi_master_put(value);
	SS_HIGH;
}

void mcp2515_bit_modify(
        const uint8_t address,
        const uint8_t mask,
        const uint8_t data) 
{
    SS_LOW;
    spi_master_put(CAN_BIT_MODIFY);
    spi_master_put(address);
    spi_master_put(mask);
    spi_master_put(data);
    SS_HIGH;
}

void mcp2515_init(const mcp2515_config_t *config) {
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
    mcp2515_write_register(
            CNF1, (1 << SJW0) | (1 << BRP0) | (1 << BRP1) | (1 << BRP2));
	mcp2515_write_register(CNF2, (1 << BTLMODE) | (1 << PHSEG11));
	mcp2515_write_register(CNF3, (1 << PHSEG21));
    mcp2515_write_register(CANINTE, mcp2515_get_interrupt_val(&config->ie));
    mcp2515_write_register(CANCTRL, (config->mode << 5));
    mcp2515_write_register(RXB0CTRL, (1 << 6) | (1 << 5));
    _delay_us(10);
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

void mcp2515_transmit(const mcp2515_frame_t *tr) {
    uint8_t tx_buf_offset = 0x10 * tr->tx;
    SS_LOW;
    spi_master_put(CAN_WRITE);
    spi_master_put(TXB0CTRL + tx_buf_offset);
    spi_master_put(0x0);
    if (tr->frame == STANDARD) {
        uint8_t id_h = (tr->id & 0x07ff) >> 3;
        uint8_t id_l = (tr->id & 0x0007);
        spi_master_put(id_h);
        spi_master_put((id_l << 5));
        spi_master_put(0x0);
        spi_master_put(0x0);
    // TODO: Implement EXTENDED CAN FRAMES
    } else if (tr->frame == EXTENDED) {
        uint8_t id_h = 0;
        uint8_t id_m = 0;
        uint8_t id_l = 0;
        // Set EXIDE to 1 and set EID17 and EID16.
        spi_master_put(0x0);
        spi_master_put((1 << 3) & id_h);
        spi_master_put(id_m);
        spi_master_put(id_l);
    } else {
        SS_HIGH;
        return;
    }
    spi_master_put(tr->data_sz & 0x0f);
    for (int i = 0; i < 8; ++i) {
        spi_master_put(tr->data[i]);
    }
    SS_HIGH;
    mcp2515_write_register(
            TXB0CTRL + tx_buf_offset, 
            (1 << TXREQ) | (1 << TXP1) | (1 << TXP0));
}

int8_t mcp2515_receive(mcp2515_frame_t *frame) {
    while(!(mcp2515_read_register(CANINTF) & (1 << RX0IF)));
    uint8_t sidl = mcp2515_read_register(RXB0SIDL);
    if ((sidl & (1 << 3)) == 0) {
        // Standard frame.
        uint16_t id_h = mcp2515_read_register(RXB0SIDH);
        frame->id = id_h << 3;
        frame->id |= (sidl & 0xe0) >> 5;
        frame->frame = STANDARD;
    } else {
        // TODO: Implement extended.
        frame->frame = EXTENDED;
    }
    uint8_t sz = mcp2515_read_register(RXB0DLC) & 0x0f;
    frame->data_sz = sz;
    for (int i = 0; i < sz; ++i) {
        frame->data[i] = mcp2515_read_register(RXB0D0 + i);
    }
    mcp2515_bit_modify(CANINTF, (1 << RX0IF), 0);
    return 0;
}
