#ifndef _MCP2515_STRUCTS_HPP_
#define _MCP2515_STRUCTS_HPP_

#include <stdbool.h>

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

typedef struct {
    bool MER_RE;
    bool WAK_IE;
    bool ERR_IE;
    bool TX2_IE;
    bool TX1_IE;
    bool TX0_IE;
    bool RX1_IE;
    bool RX0_IE;
} mcp2515_interrupts_t;

typedef struct {
    CAN_SPEED speed;
    CAN_MODE  mode;
    mcp2515_interrupts_t ie;
} mcp2515_config_t;

typedef enum {
    TX0 = 0,
    TX1,
    TX2
} TX_BUFFER;

typedef enum {
    RX0 = 0,
    RX1
} RX_BUFFER;

typedef enum {
    STANDARD = 0,
    EXTENDED = 1
} CAN_FRAME;

typedef struct {
    uint32_t id;
    uint8_t data_sz;
    uint8_t data[8];
    TX_BUFFER tx;
    CAN_FRAME frame;
} mcp2515_frame_t;

#endif
