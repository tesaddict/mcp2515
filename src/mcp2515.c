#include "mcp2515.h"

static void    spi_init(void);
static uint8_t spi_write(const uint8_t data);
// Private
static uint8_t mcp2515_get_interrupt_val(const mcp2515_interrupts_t *it);
static void    mcp2515_reset(void);
static uint8_t mcp2515_read(const uint8_t address);
static void    mcp2515_write(const uint8_t address, const uint8_t value);
static uint8_t mcp2515_read_status(void);
static void    mcp2515_bit_modify(
                   const uint8_t address, 
                   const uint8_t mask,
                   const uint8_t data);

static void spi_init(void) {
	DDRB = (1 << PINB2) | (1 << PINB3) | (1 << PINB5);
    SPCR    = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (1 << SPR0);
}

static uint8_t spi_write(const uint8_t data) {
    SPDR = data;
    while(!(SPSR & (1 << SPIF)));
    return SPDR;
}

static uint8_t mcp2515_get_interrupt_val(const mcp2515_interrupts_t *it) {
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

static void mcp2515_reset(void) {
	SS_LOW;
    spi_write(CAN_RESET);
	SS_HIGH;
}

static uint8_t mcp2515_read(uint8_t address) {
	SS_LOW;
    spi_write(CAN_READ);
    spi_write(address);
    uint8_t ret = spi_write(0xff);
	SS_HIGH;
    return ret;
}

static void mcp2515_write(const uint8_t address, const uint8_t value) {
	SS_LOW;
    spi_write(CAN_WRITE);
    spi_write(address);
    spi_write(value);
	SS_HIGH;
}

static uint8_t mcp2515_read_status() {
	SS_LOW;
    spi_write(CAN_READ_STATUS);
    uint8_t ret = spi_write(0xff);
	SS_HIGH;
    return ret;
}

static void mcp2515_bit_modify(
        const uint8_t address,
        const uint8_t mask,
        const uint8_t data) 
{
    SS_LOW;
    spi_write(CAN_BIT_MODIFY);
    spi_write(address);
    spi_write(mask);
    spi_write(data);
    SS_HIGH;
}

void mcp2515_init(const mcp2515_config_t *config) {
    spi_init();
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
    mcp2515_write(
            CNF1, (1 << SJW0) | (1 << BRP0) | (1 << BRP1) | (1 << BRP2));
	mcp2515_write(CNF2, (1 << BTLMODE) | (1 << PHSEG11));
	mcp2515_write(CNF3, (1 << PHSEG21));
    mcp2515_write(CANINTE, mcp2515_get_interrupt_val(&config->ie));
    mcp2515_write(CANCTRL, (config->mode << 5));
    mcp2515_write(RXB0CTRL, (1 << 6) | (1 << 5));
    _delay_us(10);
}

void mcp2515_send(const mcp2515_frame_t *tr) {
    uint8_t tx_buf_offset = 0x10 * tr->tx;
    SS_LOW;
    spi_write(CAN_WRITE);
    spi_write(TXB0CTRL + tx_buf_offset);
    spi_write(0x0);
    if (tr->frame == STANDARD) {
        uint8_t id_h = (tr->id & 0x07ff) >> 3;
        uint8_t id_l = (tr->id & 0x0007);
        spi_write(id_h);
        spi_write((id_l << 5));
        spi_write(0x0);
        spi_write(0x0);
    // TODO: Implement EXTENDED CAN FRAMES
    } else if (tr->frame == EXTENDED) {
        uint8_t id_h = 0;
        uint8_t id_m = 0;
        uint8_t id_l = 0;
        // Set EXIDE to 1 and set EID17 and EID16.
        spi_write(0x0);
        spi_write((1 << 3) & id_h);
        spi_write(id_m);
        spi_write(id_l);
    } else {
        SS_HIGH;
        return;
    }
    spi_write(tr->data_sz & 0x0f);
    for (int i = 0; i < 8; ++i) {
        spi_write(tr->data[i]);
    }
    SS_HIGH;
    mcp2515_write(
            TXB0CTRL + tx_buf_offset, 
            (1 << TXREQ) | (1 << TXP1) | (1 << TXP0));
}

int8_t mcp2515_recv(mcp2515_frame_t *frame) {
    while(!(mcp2515_read(CANINTF) & (1 << RX0IF)));
    uint8_t sidl = mcp2515_read(RXB0SIDL);
    if ((sidl & (1 << 3)) == 0) {
        // Standard frame.
        uint16_t id_h = mcp2515_read(RXB0SIDH);
        frame->id = id_h << 3;
        frame->id |= (sidl & 0xe0) >> 5;
        frame->frame = STANDARD;
    } else {
        // TODO: Implement extended.
        frame->frame = EXTENDED;
    }
    uint8_t sz = mcp2515_read(RXB0DLC) & 0x0f;
    frame->data_sz = sz;
    for (int i = 0; i < sz; ++i) {
        frame->data[i] = mcp2515_read(RXB0D0 + i);
    }
    mcp2515_bit_modify(CANINTF, (1 << RX0IF), 0);
    return 0;
}

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
