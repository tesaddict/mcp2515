#ifndef _MCP2515_H_
#define _MCP2515_H_
#include <avr/io.h>
#include <util/delay.h>
#include "mcp2515_structs.h"
#include "mcp2515_definitions.h"

void mcp2515_init(const mcp2515_config_t *config);
void mcp2515_send(const mcp2515_frame_t *tr);
int8_t mcp2515_recv(mcp2515_frame_t *frame);
void mcp2515_config_init(mcp2515_config_t *config, const CAN_SPEED speed, const CAN_MODE  mode);
#endif
