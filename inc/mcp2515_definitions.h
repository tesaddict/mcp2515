#ifndef _MCP2515_DEFINITIONS_H_
#define _MCP2515_DEFINITIONS_H_

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

// Transmission Registers
#define TXB0SIDH 0x31
#define TXB0SIDL 0x32
#define TXB0EID8 0x33
#define TXB0EID0 0x34

#define TXB0DLC  0x35
#define RTR      0x06
#define DLC3     0x03
#define DLC2     0x02
#define DLC1     0x01
#define DLC0     0x00

#define TXB0D0   0x36
#define TXB0D1   0x37
#define TXB0D2   0x38
#define TXB0D3   0x39
#define TXB0D4   0x3a
#define TXB0D5   0x3b
#define TXB0D6   0x3c
#define TXB0D7   0x3d

// Receive Registers
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62
#define RXB0EID8 0x63
#define RXB0EID0 0x64
#define RXB0DLC  0x65
#define RXB0D0   0x66
#define RXB0D1   0x67
#define RXB0D2   0x68
#define RXB0D3   0x69
#define RXB0D4   0x6a
#define RXB0D5   0x6b
#define RXB0D6   0x6c
#define RXB0D7   0x6d

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
#endif
