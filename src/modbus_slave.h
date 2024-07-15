/**
 * @file modbus_slave.h
 * @author niwciu (niwciu@gmail.com)
 * @brief Declarations of Modbus slave functions for handling communication and data management.
 * @date 2024-05-31
 *
 * This header file contains the declarations of functions and types related to Modbus slave
 * communication. It provides interfaces for initializing the Modbus slave, registering application
 * data to Modbus tables (coils, discrete inputs, input registers, holding registers), and checking
 * Modbus requests.
 *
 * Functions defined in this header are intended to work with a Modbus RTU driver for serial communication,
 * providing necessary functionality for Modbus slave devices to communicate with a Modbus master.
 *
 * @see modbus_slave.c for the corresponding implementation file.
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_SLAVE_H_
#define _MODBUS_SLAVE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_public_type.h"
    void register_app_data_to_modbus_slave_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_modbus_slave_din_table(modbus_adr_t disin_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_modbus_slave_inreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr);
    void register_app_data_to_modbus_slave_hreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr);

    void modbus_slave_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity, modbus_device_ID_t slave_ID);
    void check_modbus_request(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_SLAVE_H_ */