#ifndef _MCP2515_H_
#define _MCP2515_H_
#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"
#include "mcp2515_structs.h"

void mcp2515_config_init(
        mcp2515_config_t *config,
        const CAN_SPEED speed,
        const CAN_MODE  mode);

uint8_t mcp2515_get_interrupt_val(const mcp2515_interrupts_t *it);
void    mcp2515_reset(void);
void    mcp2515_init(const mcp2515_config_t *config);
uint8_t mcp2515_read_register(const uint8_t address);
void    mcp2515_write_register(
            const uint8_t address, 
            const uint8_t value);
void    mcp2515_bit_modify(
            const uint8_t address, 
            const uint8_t mask,
            const uint8_t data);
uint8_t mcp2515_read_status(void);

void mcp2515_transmit(const mcp2515_frame_t *tr);
int8_t mcp2515_receive(mcp2515_frame_t *frame);

#endif
