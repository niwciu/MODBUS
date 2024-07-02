/**
 * @file modbus_PDU_common.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 0.0.1
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_PDU_COMMON_H_
#define _MODBUS_PDU_COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_type.h"

    modbus_byte_count_t get_coil_din_byte_count(modbus_data_qty_t coil_qty);
    void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty);
    // modbus_ret_t set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_COMMON_H_ */