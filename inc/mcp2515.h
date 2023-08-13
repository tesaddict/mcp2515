#ifndef _MCP2515_H_
#define _MCP2515_H_

#include <stdint.h>

typedef enum {
  KBPS_125
} CAN_SPEED;

typedef enum {
  CONFIG   = 4,
  NORMAL   = 0,
  SLEEP    = 1,
  LISTEN   = 3,
  LOOPBACK = 2
} CAN_MODE;

typedef void (*mcp2515_spi_write_t) (const uint8_t*, const uint8_t);
typedef void (*mcp2515_spi_read_t)  (uint8_t*, const uint8_t);

typedef struct {
  CAN_SPEED speed;
  CAN_MODE  mode;
  uint8_t   interrupts;
} mcp2515_config_t;

void   mcp2515_init(const CAN_SPEED speed, 
                    const CAN_MODE mode, 
                    mcp2515_spi_write_t write,
                    mcp2515_spi_read_t read);
void    mcp2515_send(const uint8_t *data, const uint8_t size);
int32_t mcp2515_recv(uint8_t *data, uint8_t *size);
void    mcp2515_set_id(uint16_t id);

// Commands
#define CAN_RESET       0xC0U
#define CAN_READ        0x03U

#define CAN_READ_RX_BUFFER_RXB0SIDH 0x90U
#define CAN_READ_RX_BUFFER_RXB0D0   0x92U
#define CAN_READ_RX_BUFFER_RXB1SIDH 0x94U
#define CAN_READ_RX_BUFFER_RXB1D0   0x96U

#define CAN_WRITE       0x02U
#define CAN_READ_STATUS 0xA0U
#define CAN_RX_STATUS   0xB0U
#define CAN_BIT_MODIFY  0x05U

// Registers
#define BFPCTRL     0x0CU
#define TXRTSCTRL   0x0DU
#define CANSTAT     0x0EU
#define CANCTRL     0x0FU
#define TEC         0x1CU
#define REC         0x1DU
#define CNF3        0x28U
#define CNF2        0x29U
#define CNF1        0x2AU
#define CANINTE     0x2BU
#define CANINTF     0x2CU
#define EFLG        0x2DU
#define TXB0CTRL    0x30U
#define TXB1CTRL    0x40U
#define TXB2CTRL    0x50U
#define RXB0CTRL    0x60U
#define RXB1CTRL    0x70U

// Transmission Registers
#define TXB0SIDH 0x31U
#define TXB0SIDL 0x32U
#define TXB0EID8 0x33U
#define TXB0EID0 0x34U

#define TXB0DLC  0x35U
#define RTR      0x06U
#define DLC3     0x03U
#define DLC2     0x02U
#define DLC1     0x01U
#define DLC0     0x00U

#define TXB0D0   0x36U
#define TXB0D1   0x37U
#define TXB0D2   0x38U
#define TXB0D3   0x39U
#define TXB0D4   0x3AU
#define TXB0D5   0x3BU
#define TXB0D6   0x3CU
#define TXB0D7   0x3DU

// Receive Registers
#define RXB0SIDH 0x61U
#define RXB0SIDL 0x62U
#define RXB0EID8 0x63U
#define RXB0EID0 0x64U
#define RXB0DLC  0x65U
#define RXB0D0   0x66U
#define RXB0D1   0x67U
#define RXB0D2   0x68U
#define RXB0D3   0x69U
#define RXB0D4   0x6AU
#define RXB0D5   0x6BU
#define RXB0D6   0x6CU
#define RXB0D7   0x6DU

// Bit Positions

// BFPCTRL
#define B1BFS       0x05U
#define B0BFS       0x04U
#define B1BFE       0x03U
#define B0BFE       0x02U
#define B1BFM       0x01U
#define B0BFM       0x00U

// TXRTSCTRL
#define B2RTS       0x05U
#define B1RTS       0x04U
#define B0RTS       0x03U
#define B2RTSM      0x02U
#define B1RTSM      0x01U
#define B0RTSM      0x00U

// CANSTAT
#define OPMOD2      0x07U
#define OPMOD1      0x06U
#define OPMOD0      0x05U
#define ICOD2       0x03U
#define ICOD1       0x02U
#define ICOD0       0x01U

// CANCTRL
#define REQOP2      0x07U
#define REQOP1      0x06U
#define REQOP0      0x05U
#define ABAT        0x04U
#define OSM         0x03U
#define CLKEN       0x02U
#define CLKPRE1     0x01U
#define CLKPRE0     0x00U

// CNF3
#define SOF         0x07U
#define WAKFIL      0x06U
#define PHSEG22     0x02U
#define PHSEG21		  0x01U
#define PHSEG20     0x00U

// CNF2
#define BTLMODE     0x04U
#define SAM         0x06U
#define PHSEG12     0x05U
#define PHSEG11     0x04U
#define PHSEG10     0x03U
#define PRSEG2      0x02U
#define PRSEG1      0x01U
#define PRSEG0      0x00U

// CNF1
#define SJW1        0x07U
#define SJW0        0x06U
#define BRP5        0x05U
#define BRP4        0x04U
#define BRP3        0x03U
#define BRP2        0x02U
#define BRP1        0x01U
#define BRP0        0x00U

// CANINTE
#define MERRE       0x07U
#define WAKIE       0x06U
#define ERRIE       0x05U
#define TX2IE       0x04U
#define TX1IE       0x03U
#define TX0IE       0x02U
#define RX1IE       0x01U
#define RX0IE       0x00U

// CANINTF
#define MERRF       0x07U
#define WAKIF       0x06U
#define ERRIF       0x05U
#define TX2IF       0x04U
#define TX1IF       0x03U
#define TX0IF       0x02U
#define RX1IF       0x01U
#define RX0IF       0x00U

// EFLG
#define RX1OVR      0x07U
#define RX0OVR      0x06U
#define TXBO        0x05U
#define TXEP        0x04U
#define RXEP        0x03U
#define TXWAR       0x02U
#define RXWAR       0x01U
#define EWARN       0x00U

// TXB0CTRL, TXB1CTRL, TXB2CTRL
#define ABTF        0x06U
#define MLOA        0x05U
#define TXERR       0x04U
#define TXREQ       0x03U
#define TXP1        0x01U
#define TXP0        0x00U

// RXB0CTRL, RXB1CTRL
#define RXM1        0x06U
#define RXM0        0x05U
#define RXRTR       0x03U
#define BUKT        0x02U
#define BUKT1       0x01U
#define FILHIT0     0x00U

// RXB1CTRL
#define FILHIT2     0x02U
#define FILHIT1     0x01U
#define FILHIT0     0x00U

#endif
