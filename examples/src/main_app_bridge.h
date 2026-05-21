/**
 * @file main_app_bridge.h
 * @author niwciu (niwciu@gmail.com)
 * @brief Common Modbus bridge application interface.
 *        The bridge runs as a Modbus slave on one USART (upstream network) and
 *        as a Modbus master on a second USART (downstream subnet), transparently
 *        forwarding collected downstream data to the upstream master.
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include "modbus_slave.h"
#include "modbus_master.h"

void main_app_bridge(baud_t slave_baud, baud_t master_baud);
void main_app_bridge_1ms_tick(void);
