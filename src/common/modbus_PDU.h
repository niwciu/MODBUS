/**
 * @file modbus_PDU_common.h
 * @brief Header file for common Modbus Protocol Data Unit (PDU) operations.
 *
 * This header file declares functions related to Modbus Protocol Data Unit (PDU) operations,
 * specifically for coil and discrete input data handling.
 *
 * @author niwciu (niwciu@gmail.com)
 * @version 0.0.1
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
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

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_COMMON_H_ */