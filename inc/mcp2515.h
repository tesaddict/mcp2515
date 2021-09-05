#ifndef _MCP2515_H_
#define _MCP2515_H_

// Commands
#define CAN_RESET       0xc0
#define CAN_READ        0x03
#define CAN_WRITE       0x02
#define CAN_READ_STATUS 0xa0
#define CAN_RX_STATUS   0xb0
#define CAN_BIT_MODIFY  0x05

// Registers
#define BFPCTRL     0x0c
#define TXRTSCTRL   0x0d
#define CANSTAT     0x0e
#define CANCTRL     0x0f
#define TEC         0x1c
#define REC         0x1d
#define CNF3        0x28
#define CNF2        0x29
#define CNF1        0x2a
#define CANINTE     0x2b
#define CANINTF     0x2c
#define EFLG        0x2d
#define TXB0CTRL    0x30
#define TXB1CTRL    0x40
#define TXB2CTRL    0x50
#define RXB0CTRL    0x60
#define RXB1CTRL    0x70

// Bit Positions

// BFPCTRL
#define B1BFS       0x05
#define B0BFS       0x04
#define B1BFE       0x03
#define B0BFE       0x02
#define B1BFM       0x01
#define B0BFM       0x00

// TXRTSCTRL
#define B2RTS       0x05
#define B1RTS       0x04
#define B0RTS       0x03
#define B2RTSM      0x02
#define B1RTSM      0x01
#define B0RTSM      0x00

// CANSTAT
#define OPMOD2      0x07
#define OPMOD1      0x06
#define OPMOD0      0x05
#define ICOD2       0x03
#define ICOD1       0x02
#define ICOD0       0x01

// CANCTRL
#define REQOP2      0x07
#define REQOP1      0x06
#define REQOP0      0x05
#define ABAT        0x04
#define OSM         0x03
#define CLKEN       0x02
#define CLKPRE1     0x01
#define CLKPRE0     0x00

// CNF3
#define SOF         0x07
#define WAKFIL      0x06
#define PHSEG22     0x02
#define PHSEG21		0x01
#define PHSEG20     0x00

// CNF2
#define BTLMODE     0x04
#define SAM         0x06
#define PHSEG12     0x05
#define PHSEG11     0x04
#define PHSEG10     0x03
#define PRSEG2      0x02
#define PRSEG1      0x01
#define PRSEG0      0x00

// CNF1
#define SJW1        0x07
#define SJW0        0x06
#define BRP5        0x05
#define BRP4        0x04
#define BRP3        0x03
#define BRP2        0x02
#define BRP1        0x01
#define BRP0        0x00

// CANINTE
#define MERRE       0x07
#define WAKIE       0x06
#define ERRIE       0x05
#define TX2IE       0x04
#define TX1IE       0x03
#define TX0IE       0x02
#define RX1IE       0x01
#define RX0IE       0x00

// CANINTF
#define MERRF       0x07
#define WAKIF       0x06
#define ERRIF       0x05
#define TX2IF       0x04
#define TX1IF       0x03
#define TX0IF       0x02
#define RX1IF       0x01
#define RX0IF       0x00

// EFLG
#define RX1OVR      0x07
#define RX0OVR      0x06
#define TXBO        0x05
#define TXEP        0x04
#define RXEP        0x03
#define TXWAR       0x02
#define RXWAR       0x01
#define EWARN       0x00

// TXB0CTRL, TXB1CTRL, TXB2CTRL
#define ABTF        0x06
#define MLOA        0x05
#define TXERR       0x04
#define TXREQ       0x03
#define TXP1        0x01
#define TXP0        0x00

// RXB0CTRL, RXB1CTRL
#define RXM1        0x06
#define RXM0        0x05
#define RXRTR       0x03
#define BUKT        0x02
#define BUKT1       0x01
#define FILHIT0     0x00

// RXB1CTRL
#define FILHIT2     0x02
#define FILHIT1     0x01
#define FILHIT0     0x00

#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"
#include "definitions.h"

void    mcp2515_reset(void);
void    mcp2515_init(void);
uint8_t mcp2515_read_register(uint8_t address);
void    nco2515_write_register(uint8_t address, uint8_t value);
uint8_t mcp2515_read_status(void);

#endif
