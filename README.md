# MCP2515 Atmega328P Driver

This is a side project to familiarize myself with both the MCP2515 CAN Bus module and
the Atmega328P MCU. After all MCP2515 functionality has been implemented; I intend to
make the driver generic for any platform that communicates via SPI.

## MCP2515 Configuration

The current version supports configuring the CAN Bus speed, CAN mode and interrupt
register CANINTE. Only 125 Kbps is supported. All modes and interrupts can be set.

Acceptance masks, filters and extended frames will be added in a future update.

```
/* MCP2515 Configuration Struct */
typedef struct {
    CAN_SPEED speed;
    CAN_MODE  mode;
    mcp2515_interrupts_t ie;
} mcp2515_config_t;

/* MCP2515 Initialization example */

mcp2515_config_t config;
/* 125 Kbps in LOOPBACK mode.
mcp2515_config_init(&config, KBPS_125, LOOPBACK);
mcp2515_init(&config);
```

## MCP2515 Usage

Both send and receive have been implemented. For the time being, only the RX0 and TX0
buffers are used. 

```
typedef struct {
    uint32_t id;
    uint8_t data_sz;
    uint8_t data[8];
    CAN_FRAME frame;
} mcp2515_frame_t;

/* Example send */
mcp2515_frame_t send_frame;
send_frame.id = 42;
send_frame.data_sz = 5;
send_frame.data[0] = 'H';
send_frame.data[1] = 'e';
send_frame.data[2] = 'l';
send_frame.data[3] = 'l';
send_frame.data[4] = 'o';
send_frame.frame = STANDARD;
mcp2515_send(&send_frame);

/* Example receive */
mcp2515_frame_t recv_frame;
int8_t ret = mcp2515_recv(&recv_frame);
```

