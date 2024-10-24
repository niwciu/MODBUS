
/**
 * @file modbus_master.h
 * @brief Declarations for Modbus master functions and initialization.
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-27
 * @copyright Copyright (c) 2024
 *
 * This header file provides declarations for Modbus master functions and initialization routines.
 * It includes functions for reading and writing coils (discrete outputs) and registers (input and holding),
 * as well as initializing the Modbus master configuration and managing Modbus responses.
 *
 * Functions like `modbus_master_read_coils` and `modbus_master_write_single_reg` are used to perform
 * Modbus transactions with a specified slave device identified by `slave_ID` and targeted data addresses (`adr`).
 *
 * The `modbus_master_init` function initializes the Modbus master with specified communication parameters
 * such as mode (`mode`), baud rate (`baud_rate`), and parity (`parity`). The `update_modbus_master_manager` function,
 * although suggested for renaming, is intended to manage Modbus responses or slave communications.
 *
 * @note Ensure correct usage of these functions according to the Modbus protocol specifications and your
 *       application's requirements for reliable communication with Modbus slave devices.
 */
#ifndef _MODBUS_MASTER_H_
#define _MODBUS_MASTER_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_public_type.h"

    // void register_modbus_master_error_cb(modbus_master_error_cb_t error_callback);
    modbus_master_req_ret_t modbus_master_read_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID);
    modbus_master_req_ret_t modbus_master_read_discrete_inputs(modbus_adr_t adr, modbus_data_qty_t discrete_input_qty, modbus_device_ID_t slave_ID);
    modbus_master_req_ret_t modbus_master_read_input_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID);
    modbus_master_req_ret_t modbus_master_read_holding_reg(modbus_adr_t adr, modbus_data_qty_t hreg_qty, modbus_device_ID_t slave_ID);

    modbus_master_req_ret_t modbus_master_write_single_coil(modbus_adr_t adr, modbus_device_ID_t slave_ID, modbus_coil_disin_t coil_2_write);
    modbus_master_req_ret_t modbus_master_write_single_reg(modbus_adr_t adr, modbus_device_ID_t slave_ID, modbus_reg_t reg_2_write);

    modbus_master_req_ret_t modbus_master_write_multiple_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID, modbus_reg_t *rw_data_ptr);
    modbus_master_req_ret_t modbus_master_write_multiple_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID, modbus_coil_disin_t *rw_data_ptr);

    void modbus_master_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity);
    void update_modbus_master_manager(void); // ToDo zmiana nazwy na check_slave_respo albo master_parse_modbus_resp albo check_modbus_master_resp
    void update_modbus_master_timout_timer(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_MASTER_H_ */