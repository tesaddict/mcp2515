#include "mcp2515.h"
#include <stdint.h>

static mcp2515_config_t mcp2515;
static mcp2515_spi_transaction_t transact;

static void mcp2515_reset(void) {
  #define MCP2515_RESET_SZ 1U
  uint8_t cmd[MCP2515_RESET_SZ] = { CAN_RESET };
  transact(cmd, MCP2515_RESET_SZ);
}

static void mcp2515_write(const uint8_t address, const uint8_t value) {
  #define MCP2515_WRITE_SZ 3U
  uint8_t cmd[MCP2515_WRITE_SZ] = { CAN_WRITE, address, value };
  transact(cmd, MCP2515_WRITE_SZ);
}

static uint8_t mcp2515_read(const uint8_t address) {
  #define MCP2515_READ_SZ 3U
  uint8_t cmd[MCP2515_READ_SZ] = { CAN_READ, address, 0xFF };
  transact(cmd, MCP2515_READ_SZ);
  return cmd[2];
}

void mcp2515_init(const CAN_SPEED speed, 
                  const CAN_MODE mode, 
                  mcp2515_spi_transaction_t tr) 
{
  mcp2515.speed = speed;
  mcp2515.mode  = mode;
  transact = tr;
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

void mcp2515_send(const mcp2515_frame_t *tr) {
  #define MCP2515_SEND_SZ 16U
  uint8_t cmd[MCP2515_SEND_SZ] = {
   CAN_WRITE,
   TXB0CTRL,
   0x00U,
   (tr->id & 0x07ff) >> 3,
   (tr->id & 0x0007) << 5,
   0x00U,
   0x00U,
   tr->data_size & 0x0FU,
   tr->data[0], tr->data[1], tr->data[2], tr->data[3],
   tr->data[4], tr->data[5], tr->data[6], tr->data[7],
  };
  transact(cmd, MCP2515_SEND_SZ);
  mcp2515_write(TXB0CTRL, (1 << TXREQ) | (1 << TXP1) | (1 << TXP0));
}

#define MCP2515_SIDH_INDEX    1U
#define MCP2515_SIDL_INDEX    2U
#define MCP2515_EID8_INDEX    3U
#define MCP2515_EID0_INDEX    4U
#define MCP2515_DLC_INDEX     5U
#define MCP2515_DATA_INDEX    6U
#define MCP2515_RX_BUFFER_SZ 14U
int8_t mcp2515_recv(mcp2515_frame_t *frame) {
  if (!(mcp2515_read(CANINTF) & (1 << RX0IF))) return -1;
  uint8_t frame_data[MCP2515_RX_BUFFER_SZ] = { 0xFFU };
  frame_data[0] = CAN_READ_RX_BUFFER_RXB0D0;
  (*transact)(frame_data, MCP2515_RX_BUFFER_SZ);
  frame->id = (frame_data[MCP2515_SIDH_INDEX] << 3) | (frame_data[MCP2515_SIDL_INDEX] >> 5);
  frame->data_size = frame_data[MCP2515_DLC_INDEX] & 0x0FU;
  for (uint8_t i = 0; i < frame->data_size; ++i) {
    frame->data[i] = frame_data[MCP2515_DATA_INDEX + i];
  }
  return 0;
}