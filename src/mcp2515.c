#include "mcp2515.h"
#include <stdint.h>


static mcp2515_config_t mcp2515;

#define MCP2515_SEND_SZ 16U
static uint8_t out[MCP2515_SEND_SZ] = {
  CAN_WRITE, TXB0CTRL,
  0x00U, 0x00U,
  0x00U, 0x00U,
  0x00U, 0x00U,
  0x00U, 0x00U,
  0x00U, 0x00U,
  0x00U, 0x00U,
  0x00U, 0x00U
};
  
#define MCP2515_RX_BUFFER_SZ 15U
static uint8_t in[MCP2515_RX_BUFFER_SZ] = { 0xFFU };

static mcp2515_spi_write_t g_write;
static mcp2515_spi_read_t g_read;

static void mcp2515_reset(void) {
  #define MCP2515_RESET_SZ 1U
  const uint8_t cmd[MCP2515_RESET_SZ] = { CAN_RESET };
  g_write(cmd, MCP2515_RESET_SZ);
}

static void mcp2515_write(const uint8_t address, const uint8_t value) {
  #define MCP2515_WRITE_SZ 3U
  const uint8_t cmd[MCP2515_WRITE_SZ] = { CAN_WRITE, address, value };
  g_write(cmd, MCP2515_WRITE_SZ);
}

static uint8_t mcp2515_read(const uint8_t address) {
  #define MCP2515_READ_SZ 3U
  uint8_t cmd[MCP2515_READ_SZ] = { CAN_READ, address, 0xFF };
  g_read(cmd, MCP2515_READ_SZ);
  return cmd[2];
}

void mcp2515_init(const CAN_SPEED speed, 
                  const CAN_MODE mode, 
                  mcp2515_spi_write_t write,
                  mcp2515_spi_read_t read) 
{
  mcp2515.speed = speed;
  mcp2515.mode  = mode;
  g_write = write;
  g_read = read;
  mcp2515_reset();
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
  mcp2515_write(CNF1, (1 << SJW0) | (1 << BRP0) | (1 << BRP1) | (1 << BRP2));
  mcp2515_write(CNF2, (1 << BTLMODE) | (1 << PHSEG11));
  mcp2515_write(CNF3, (1 << PHSEG21));
  mcp2515_write(CANINTE, 0x00);
  mcp2515_write(CANCTRL, (mcp2515.mode << 5));
  mcp2515_write(RXB0CTRL, (1 << 6) | (1 << 5));
}

void mcp2515_send(const uint8_t *data, const uint8_t size) {
  out[7]  = size & 0x0FU;
  for (uint8_t i = 0; i < size; ++i) {
    out[8 + i] = data[i];
  }
  g_write(out, MCP2515_SEND_SZ);
  mcp2515_write(out[1], (1 << TXREQ) | (1 << TXP1) | (1 << TXP0));
}

#define MCP2515_SIDH_INDEX    1U
#define MCP2515_SIDL_INDEX    2U
#define MCP2515_EID8_INDEX    3U
#define MCP2515_EID0_INDEX    4U
#define MCP2515_DLC_INDEX     5U
#define MCP2515_DATA_INDEX    6U
int32_t mcp2515_recv(uint8_t *data, uint8_t *size) {
  if (!(mcp2515_read(CANINTF) & (1 << RX0IF))) return -1;
  in[0] = CAN_READ_RX_BUFFER_RXB0SIDH;
  g_read(in, MCP2515_RX_BUFFER_SZ);
  int32_t id = (in[MCP2515_SIDH_INDEX] << 3) | (in[MCP2515_SIDL_INDEX] >> 5);
  *size = in[MCP2515_DLC_INDEX] & 0x0FU;
  for (uint8_t i = 0; i < *size; ++i) {
    data[i] = in[MCP2515_DATA_INDEX + i];
  }
  return id;
}

void mcp2515_set_id(uint16_t id) {   
  out[3] = (id & 0x07ff) >> 3;
  out[4] = (id & 0x0007) << 5;
}