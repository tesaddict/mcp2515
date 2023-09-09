#ifndef _MCP2515_H_
#define _MCP2515_H_

#include <stdint.h>

typedef enum {
  KBPS_125
} MCP2515_SPEED;

typedef enum {
  NORMAL   = 0,
  SLEEP    = 1,
  LISTEN   = 3,
  LOOPBACK = 2,
  CONFIG   = 4,
} MCP2515_MODE;

typedef enum {
  MCP2515_STATUS_SUCCESS = 0,
  MCP2515_STATUS_RECEPTION_FAILED = 1
} MCP2515_Status_t;

typedef void (*mcp2515_spi_transact_func_ptr)  (uint8_t*, const uint8_t);

typedef struct {
  MCP2515_SPEED speed;
  MCP2515_MODE  mode;
} mcp2515_config_t;

typedef struct {
  uint32_t id;
  uint8_t size;
  uint8_t data[8];
} mcp2515_frame_t;

void   mcp2515_init(const MCP2515_SPEED speed, 
                    const MCP2515_MODE mode, 
                    mcp2515_spi_transact_func_ptr transact);
void    mcp2515_send(const mcp2515_frame_t *in);
MCP2515_Status_t mcp2515_recv(mcp2515_frame_t *out);

#endif
