#include "mcp2515.h"
#include "mcp2515_defines.h"

static mcp2515_config_t mcp2515;

#define BUFFER_SIZE   14U

#define COMMAND_INDEX 0U
#define SIDH_INDEX    1U
#define SIDL_INDEX    2U
#define EID8_INDEX    3U
#define EID0_INDEX    4U
#define DLC_INDEX     5U
#define DATA_INDEX    6U
static uint8_t transmit_buffer[BUFFER_SIZE] = { 0x00U };
static uint8_t receive_buffer[BUFFER_SIZE]  = { 0x00U };

static mcp2515_spi_transact_func_ptr spi_transact;

static void mcp2515_reset(void) {
  #define MCP2515_RESET_SZ 1U
  uint8_t cmd[MCP2515_RESET_SZ] = { MCP2515_RESET };
  spi_transact(cmd, MCP2515_RESET_SZ);
}

static void mcp2515_write(const uint8_t address, const uint8_t value) {
  #define MCP2515_WRITE_SZ 3U
  uint8_t cmd[MCP2515_WRITE_SZ] = { MCP2515_WRITE, address, value };
  spi_transact(cmd, MCP2515_WRITE_SZ);
}

static uint8_t mcp2515_read(const uint8_t address) {
  #define MCP2515_READ_SZ 3U
  uint8_t cmd[MCP2515_READ_SZ] = { MCP2515_READ, address, 0xFF };
  spi_transact(cmd, MCP2515_READ_SZ);
  return cmd[2];
}

void mcp2515_init(const MCP2515_SPEED speed, 
                  const MCP2515_MODE mode, 
                  mcp2515_spi_transact_func_ptr transact) 
{
  mcp2515.speed = speed;
  mcp2515.mode  = mode;
  spi_transact = transact;
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
  mcp2515_write(MCP2515_CNF1, MCP2515_CNF1_SJW0_MASK | MCP2515_CNF1_BRP0_MASK |
                              MCP2515_CNF1_BRP1_MASK | MCP2515_CNF1_BRP2_MASK);
  mcp2515_write(MCP2515_CNF2, MCP2515_CNF2_BTLMODE_MASK | MCP2515_CNF2_PHSEG11_MASK);
  mcp2515_write(MCP2515_CNF3, MCP2515_CNF3_PHSEG21_MASK);
  mcp2515_write(MCP2515_CANINTE, 0x00);
  mcp2515_write(MCP2515_CANCTRL, (mcp2515.mode << MCP2515_CANCTRL_REQOP0_SHIFT));
  mcp2515_write(MCP2515_RXB0CTRL, MCP2515_RXB0CTRL_RXM1_MASK | MCP2515_RXB0CTRL_RXM0_MASK);
}

void mcp2515_send(const mcp2515_frame_t *in) {
  transmit_buffer[COMMAND_INDEX] = MCP2515_WRITE_TX_BUFFER_TXB0SIDH;
  transmit_buffer[SIDH_INDEX] = (in->id & 0x07ff) >> 3;
  transmit_buffer[SIDL_INDEX] = (in->id & 0x0007) << 5;
  transmit_buffer[EID8_INDEX] = 0x00U;
  transmit_buffer[EID0_INDEX] = 0x00U;
  transmit_buffer[DLC_INDEX]  = in->size & 0x0FU;
  for (uint8_t idx = 0; idx < in->size; ++idx) {
    transmit_buffer[DATA_INDEX + idx] = in->data[idx];
  }
  spi_transact(transmit_buffer, BUFFER_SIZE);
  mcp2515_write(MCP2515_TXB0CTRL, MCP2515_TXBNCTRL_TXREQ_MASK | MCP2515_TXBNCTRL_TXP1_MASK | 
                                  MCP2515_TXBNCTRL_TXP0_MASK);
}

MCP2515_Status_t mcp2515_recv(mcp2515_frame_t *out) {
  if (!(mcp2515_read(MCP2515_CANINTF) & MCP2515_CANINTF_RX0IF_MASK)) {
    return MCP2515_STATUS_RECEPTION_FAILED;
  }
  receive_buffer[COMMAND_INDEX] = MCP2515_READ_RX_BUFFER_RXB0SIDH;
  spi_transact(receive_buffer, BUFFER_SIZE);
  out->id = (receive_buffer[SIDH_INDEX] << 3) | (receive_buffer[SIDL_INDEX] >> 5);
  out->size = receive_buffer[DLC_INDEX] & 0x0FU;
  for (uint8_t idx = 0; idx < out->size; ++idx) {
    out->data[idx] = receive_buffer[DATA_INDEX + idx];
  }
  return MCP2515_STATUS_SUCCESS;
}